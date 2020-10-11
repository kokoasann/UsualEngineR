#include "PreCompile.h"
#include "GaussBlur.h"



namespace UER
{
	void GaussBlur::Release()
	{
		
	}
	void GaussBlur::Init(int w, int h, RenderTarget* inTex, int inW, int inH, RenderTarget*& outTex)
	{

		m_width = w>>1;
		m_height = h>>1;

		m_inWidth = inW;
		m_inHeight = inH;

		m_rtX.Create(m_width, h, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D16_UNORM, nullptr);
		m_rtY.Create(m_width, m_height, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D16_UNORM, nullptr);

		m_rtX.SetName(L"BlurX");
		m_rtY.SetName(L"BlurY");

		outTex = &m_rtY;

		m_rootSign.Init(
			D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP
		);
		m_vsX.LoadVS(L"Assets/shader/GaussBlur.fx", "VSMain_X");
		m_vsY.LoadVS(L"Assets/shader/GaussBlur.fx", "VSMain_Y");
		m_ps.LoadPS(L"Assets/shader/GaussBlur.fx", "PSMain");
		{
			D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			};
			//パイプラインステートを作成。
			D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = { 0 };
			psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
			psoDesc.pRootSignature = m_rootSign.Get();
			psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsX.GetCompiledBlob());
			psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_ps.GetCompiledBlob());
			psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
			psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
			psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
			psoDesc.DepthStencilState.DepthEnable = FALSE;
			psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
			psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
			psoDesc.DepthStencilState.StencilEnable = FALSE;
			psoDesc.SampleMask = UINT_MAX;
			psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			psoDesc.NumRenderTargets = 1;
			psoDesc.RTVFormats[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;		//アルベドカラー出力用。
			psoDesc.DSVFormat = DXGI_FORMAT_D16_UNORM;
			psoDesc.SampleDesc.Count = 1;
			m_pipStateX.Init(psoDesc);

			psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsY.GetCompiledBlob());
			m_pipStateY.Init(psoDesc);
		}
		D3D12_SAMPLER_DESC samplerDesc = 
		{
			D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP, // AddressU
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP, // AddressV
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP, // AddressW
			0, // MipLODBias
			D3D12_MAX_MAXANISOTROPY,
			D3D12_COMPARISON_FUNC_NEVER,
			{ 0, 0, 0, 0 }, // BorderColor
			0, // MinLOD
			FLT_MAX // MaxLOD
		};
		/*samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		samplerDesc.MipLODBias = 0;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		samplerDesc.BorderColor[0] = 1.0f;
		samplerDesc.BorderColor[1] = 1.0f;
		samplerDesc.BorderColor[2] = 1.0f;
		samplerDesc.BorderColor[3] = 1.0f;
		samplerDesc.MinLOD = 0.0f;
		samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;*/
		

		m_inputTex.InitFromD3DResource(inTex->GetRenderTargetTexture().Get());
		m_constBuffer[0].Init(sizeof(BlurData));
		m_constBuffer[1].Init(sizeof(BlurData));

		m_descHeapX.RegistShaderResource(0,m_inputTex);
		m_descHeapX.RegistConstantBuffer(0, m_constBuffer[0]);
		m_descHeapX.Commit();
		
		
		m_descHeapY.RegistShaderResource(0, m_rtX.GetRenderTargetTexture());
		m_descHeapY.RegistConstantBuffer(0, m_constBuffer[1]);
		m_descHeapY.Commit();

		m_samplerHeap.RegistSamplerDesc(0, samplerDesc);
		m_samplerHeap.CommitSamperHeap();
	}
	void GaussBlur::Render(RenderContext& rc,const Primitive& prim)
	{
		UpdateWeight();

		m_blurData.rttexRatio = float(m_inWidth)/float(m_width);
		//m_blurData.offset.x = -((8.f * (m_blurData.rttexRatio)) / m_width);
		m_blurData.offset.x = 8.f / m_inWidth;
		m_blurData.offset.y = 0.f;

		m_constBuffer[0].CopyToVRAM(m_blurData);

		rc.WaitUntilToPossibleSetRenderTarget(m_rtX);
		rc.SetRenderTarget(m_rtX.GetRTVCpuDescriptorHandle(), m_rtX.GetDSVCpuDescriptorHandle());
		D3D12_VIEWPORT viewport;
		viewport.Height = m_rtX.GetHeight();
		viewport.Width = m_rtX.GetWidth();
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.MinDepth = 0;
		viewport.MaxDepth = 1;

		rc.SetViewport(viewport);
		rc.SetRootSignature(m_rootSign);
		rc.SetPipelineState(m_pipStateX);
		ID3D12DescriptorHeap* heaps[] =
		{
			m_descHeapX.Get(),
			m_samplerHeap.Get()
		};
		//.SetDescriptorHeaps(2,heaps);
		rc.SetDescriptorHeap(m_descHeapX);
		
		prim.Draw(rc);
		rc.WaitUntilFinishDrawingToRenderTarget(m_rtX);


		m_blurData.rttexRatio = 2.f * (float(m_width) / float(m_height));
		m_blurData.offset.x = 0.f;
		//m_blurData.offset.y = -((8.f * (m_blurData.rttexRatio)) / (float)m_height);
		m_blurData.offset.y = 8.f / m_inHeight;

		m_constBuffer[1].CopyToVRAM(m_blurData);

		rc.WaitUntilToPossibleSetRenderTarget(m_rtY);
		rc.SetRenderTarget(m_rtY.GetRTVCpuDescriptorHandle(), m_rtY.GetDSVCpuDescriptorHandle());
		viewport.Height = m_rtY.GetHeight();
		viewport.Width = m_rtY.GetWidth();

		rc.SetViewport(viewport);
		rc.SetRootSignature(m_rootSign);
		rc.SetPipelineState(m_pipStateY);
		heaps[0] = m_descHeapY.Get();
		//rc.SetDescriptorHeaps(2, heaps);
		rc.SetDescriptorHeap(m_descHeapY);
		
		prim.Draw(rc);
		rc.WaitUntilFinishDrawingToRenderTarget(m_rtY);
	}
	void GaussBlur::UpdateWeight()
	{
		if (!m_isChangeWeight)
			return;
		if (m_dispersion == 0.f)
		{
			for (int i = 0; i < MAX_WEIGHT; i++)
			{
				m_blurData.weights[i] = 0.f;
			}
			return;
		}

		float total = 0;
		for (int i = 0; i < MAX_WEIGHT; i++) {
			m_blurData.weights[i] = expf(-0.5f * (float)(i * i) / m_dispersion);
			total += 2.0f * m_blurData.weights[i];
		}
		// 規格化
		for (int i = 0; i < MAX_WEIGHT; i++) {
			m_blurData.weights[i] /= total;
		}
		m_isChangeWeight = false;
	}
}