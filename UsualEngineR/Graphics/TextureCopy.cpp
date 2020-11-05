#include "PreCompile.h"
#include "TextureCopy.h"



namespace UER
{
	void TextureCopy::Release()
	{
	}
	void TextureCopy::Init(Texture* srcTex[3], BlendMethod bm, TextureNum texNum)
	{
		m_rootSign.Init(
			D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP
		);
		m_descHeap.RegistShaderResource(0, *srcTex[0]);
		
	
		m_vs.LoadVS(L"Assets/shader/Copy.fx", "VSMain");
		switch (texNum)
		{
		case TextureNum::Single:
			m_ps.LoadPS(L"Assets/shader/Copy.fx", "PSMain");
			break;
		case TextureNum::Double:
			m_ps.LoadPS(L"Assets/shader/Copy.fx", "PSMain_Double");
			m_descHeap.RegistShaderResource(1, *srcTex[1]);
			break;
		case TextureNum::Triple:
			m_ps.LoadPS(L"Assets/shader/Copy.fx", "PSMain_Triple");
			m_descHeap.RegistShaderResource(1, *srcTex[1]);
			m_descHeap.RegistShaderResource(2, *srcTex[2]);
			break;
		}

		m_descHeap.Commit();
		{
			D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			};
			CD3DX12_BLEND_DESC blendDescS(D3D12_DEFAULT);
			D3D12_BLEND_DESC blendDesc;
			blendDesc.AlphaToCoverageEnable = FALSE;
			blendDesc.IndependentBlendEnable = FALSE;
			D3D12_RENDER_TARGET_BLEND_DESC rtblenddesc =
			{
				TRUE,FALSE,
				//D3D12_BLEND_SRC_ALPHA,D3D12_BLEND_INV_SRC_ALPHA,D3D12_BLEND_OP_ADD,
				//D3D12_BLEND_SRC_ALPHA,D3D12_BLEND_INV_SRC_ALPHA,D3D12_BLEND_OP_ADD,
				D3D12_BLEND_ONE,D3D12_BLEND_ONE,D3D12_BLEND_OP_ADD,
				D3D12_BLEND_ONE,D3D12_BLEND_ONE,D3D12_BLEND_OP_ADD,
				D3D12_LOGIC_OP_NOOP,
				D3D12_COLOR_WRITE_ENABLE_ALL
				//((D3D12_COLOR_WRITE_ENABLE_RED | D3D12_COLOR_WRITE_ENABLE_GREEN) | D3D12_COLOR_WRITE_ENABLE_BLUE)
			};

			for (auto& blend : blendDesc.RenderTarget)
			{
				blend = rtblenddesc;
			}
			CD3DX12_RASTERIZER_DESC rastDesc(D3D12_DEFAULT);
			//rastDesc.DepthClipEnable = TRUE;
			//パイプラインステートを作成。
			D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = { 0 };
			psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
			psoDesc.pRootSignature = m_rootSign.Get();
			psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vs.GetCompiledBlob());
			psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_ps.GetCompiledBlob());
			psoDesc.RasterizerState = rastDesc;
			psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
			psoDesc.BlendState = blendDesc;
			psoDesc.DepthStencilState.DepthEnable = FALSE;
			psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
			psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
			psoDesc.DepthStencilState.StencilEnable = FALSE;
			psoDesc.SampleMask = UINT_MAX;
			psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			psoDesc.NumRenderTargets = 1;
			psoDesc.RTVFormats[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;		//アルベドカラー出力用。
			psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
			psoDesc.SampleDesc.Count = 1;


			m_pipeState.Init(psoDesc);
		}
		
	}
	void TextureCopy::Render(RenderContext& rc,RenderTarget& rt, const Primitive& prim)
	{
		//rc.WaitUntilToPossibleSetRenderTarget(rt);
		rc.SetRenderTarget(rt.GetRTVCpuDescriptorHandle(), rt.GetDSVCpuDescriptorHandle());
		D3D12_VIEWPORT vp{ 0,0,rt.GetWidth(),rt.GetHeight(),0,1 };
		rc.SetViewport(vp);

		rc.SetRootSignature(m_rootSign);
		rc.SetDescriptorHeap(m_descHeap);
		rc.SetPipelineState(m_pipeState);

		prim.Draw(rc);
		//rc.WaitUntilFinishDrawingToRenderTarget(rt);
	}
}