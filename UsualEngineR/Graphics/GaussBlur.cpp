#include "PreCompile.h"
#include "GaussBlur.h"



namespace UER
{
	void GaussBlur::Release()
	{
	}
	void GaussBlur::Init(int w, int h, Texture* inTex, Texture*& outTex)
	{
		m_width = w;
		m_height = h;

		m_rtX.Create(w<<1, h, 0, 1, DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D16_UNORM, nullptr);
		m_rtY.Create(w << 1, h<<1, 0, 1, DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D16_UNORM, nullptr);

		m_rootSign.Init(
			D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP
		);
		m_vsX.LoadVS(L"Assets/shader/GaussBlur.fx", "VSMain_X");
		m_vsY.LoadVS(L"Assets/shader/GaussBlur.fx", "VSMain_Y");
		m_ps.LoadPS(L"Assets/shader/GaussBlur.fx", "PSMain");
		{
			D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			};

			//パイプラインステートを作成。
			D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = { 0 };
			psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
			psoDesc.pRootSignature = m_rootSign.Get();
			psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsX.GetCompiledBlob());
			psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_ps.GetCompiledBlob());
			psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
			psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
			psoDesc.DepthStencilState.DepthEnable = FALSE;
			psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
			psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
			psoDesc.DepthStencilState.StencilEnable = FALSE;
			psoDesc.SampleMask = UINT_MAX;
			psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			psoDesc.NumRenderTargets = 3;
			psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;		//アルベドカラー出力用。
			m_pipStateX.Init(psoDesc);

			psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsY.GetCompiledBlob());
			m_pipStateY.Init(psoDesc);
		}
		m_inputTex.InitFromD3DResource(inTex->Get());
		m_constBuffer.Init(sizeof(BlurData));

		m_descHeapX.RegistShaderResource(0,m_inputTex);
		m_descHeapX.RegistConstantBuffer(0, m_constBuffer);
		m_descHeapX.Commit();
		
		m_descHeapY.RegistShaderResource(0, m_rtX.GetRenderTargetTexture());
		m_descHeapY.RegistConstantBuffer(0, m_constBuffer);
		m_descHeapY.Commit();
	}
	void GaussBlur::Render(RenderContext& rc)
	{
		m_blurData.rttexRatio = 2;
		m_blurData.offset.x = (8.f * (m_blurData.rttexRatio)) / m_width;
		m_blurData.offset.y = 0.f;

		m_constBuffer.CopyToVRAM(m_blurData);



		m_blurData.rttexRatio = 2.f * (float(m_width<<1) / float(m_height<<1));
		m_blurData.offset.x = 0.f;
		m_blurData.offset.y = (8.f * (m_blurData.rttexRatio)) / (float)m_height;

		m_constBuffer.CopyToVRAM(m_blurData);
	}
}