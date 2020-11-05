#include "PreCompile.h"
#include "LightStreeks.h"



namespace UER
{
	void LightStreeks::Init(float width, float height, Texture* inTex, Texture*& outTex)
	{
		float cc[4] = { 0,0,0,0 };
		m_renderTargetX.Create(width, height, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D16_UNORM, cc);
		m_renderTargetY.Create(width, height, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D16_UNORM, cc);
		m_renderTargetX.SetName(L"LightStreeksX");
		m_renderTargetY.SetName(L"LightStreeksY");

		m_rootSign.Init(
			D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP
		);
		m_vs.LoadVS(L"Assets/shader/LightStreeks.fx", "VSMain");
		m_ps.LoadPS(L"Assets/shader/LightStreeks.fx", "PSMain");
		{
			D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			};

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

			//パイプラインステートを作成。
			D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = { 0 };
			psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
			psoDesc.pRootSignature = m_rootSign.Get();
			psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vs.GetCompiledBlob());
			psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_ps.GetCompiledBlob());
			psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
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
			psoDesc.DSVFormat = DXGI_FORMAT_D16_UNORM;
			psoDesc.SampleDesc.Count = 1;
			m_pipState.Init(psoDesc);

			m_constBufferX.Init(sizeof(SConstBuffData));
			m_constBufferY.Init(sizeof(SConstBuffData));

			m_descHeapX.RegistConstantBuffer(0, m_constBufferX);
			m_descHeapX.RegistShaderResource(0, m_renderTargetX.GetRenderTargetTexture());
			m_descHeapX.Commit();

			m_descHeapY.RegistConstantBuffer(0, m_constBufferY);
			m_descHeapY.RegistShaderResource(0, m_renderTargetX.GetRenderTargetTexture());
			m_descHeapY.Commit();

			outTex = &m_renderTargetY.GetRenderTargetTexture();
		}
		m_texCopy.Init(&inTex, TextureCopy::BlendMethod::BM_Add, TextureCopy::TextureNum::Single);
	}
	void LightStreeks::Draw(RenderContext& rc,const Primitive& prim)
	{
		rc.WaitUntilToPossibleSetRenderTarget(m_renderTargetX);
		m_renderTargetX.Clear(rc);
		m_texCopy.Render(rc, m_renderTargetX, prim);
		rc.WaitUntilFinishDrawingToRenderTarget(m_renderTargetX);

		rc.WaitUntilToPossibleSetRenderTarget(m_renderTargetY);
		rc.SetRenderTargets(1,&m_renderTargetY);
		m_renderTargetY.Clear(rc);

		m_constBufferX.CopyToVRAM(m_constBuffDataX);
		
		rc.SetRootSignature(m_rootSign);
		rc.SetDescriptorHeap(m_descHeapX);
		rc.SetPipelineState(m_pipState);
		prim.Draw(rc);

		
		//rc.WaitUntilFinishDrawingToRenderTarget(m_renderTargetX);
		//rc.WaitUntilToPossibleSetRenderTarget(m_renderTargetY);
		//m_renderTargetY.Clear(rc);
		//rc.CopyResource(m_renderTargetY.GetRenderTargetTexture().Get(), m_renderTargetX.GetRenderTargetTexture().Get());

		m_constBufferY.CopyToVRAM(m_constBuffDataY);
		rc.SetDescriptorHeap(m_descHeapY);
		prim.Draw(rc);

		rc.WaitUntilFinishDrawingToRenderTarget(m_renderTargetY);
	}
	void LightStreeks::UpdateWeight(float d)
	{
		if (d == 0.f)
		{
			for (int i = 0; i < 8; i++)
			{
				m_constBuffDataX.weights[i] = 0.f;
				m_constBuffDataY.weights[i] = 0.f;
			}
			return;
		}

		float total = 0;
		for (int i = 0; i < 8; i++) {
			m_constBuffDataX.weights[i] = expf(-0.5f * (float)(i * i) / d);
			total += 2.0f * m_constBuffDataX.weights[i];
		}
		// 規格化
		for (int i = 0; i < 8; i++) {
			m_constBuffDataX.weights[i] /= total;
			m_constBuffDataY.weights[i] = m_constBuffDataX.weights[i];
		}
		
	}
}