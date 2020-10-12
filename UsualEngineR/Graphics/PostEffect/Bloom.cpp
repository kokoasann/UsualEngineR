#include "PreCompile.h"
#include "Bloom.h"

#include "PostEffect.h"

namespace UER
{
	void Bloom::Release()
	{
	}

	void Bloom::Init(PostEffect* pe)
	{
		m_postEffect = pe;

		float clearColor[4] = { 0,0,0,0 };
		m_combineRT.Create(FRAME_BUFFER_W, FRAME_BUFFER_H, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D16_UNORM, clearColor);
		m_luminanceRT.Create(FRAME_BUFFER_W, FRAME_BUFFER_H, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D16_UNORM, clearColor);

		m_combineRT.SetName(L"BloomCombineRT");
		m_luminanceRT.SetName(L"BloomLuminanceRT");

		auto* currentRT = &m_luminanceRT;
		for (int i = 0; i < MAX_GAUSS; i++)
		{
			m_blur[i].Init(FRAME_BUFFER_W>>i, FRAME_BUFFER_H>>i, currentRT, currentRT->GetWidth(), currentRT->GetHeight(), currentRT);
		}

		m_rootSign.Init(
			D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP
		);

		m_vsBloom.LoadVS(L"Assets/shader/Bloom.fx", "VSMain");
		m_psLuminance.LoadPS(L"Assets/shader/Bloom.fx", "PSMain_SamplingLuminance");
		m_psCombine.LoadPS(L"Assets/shader/Bloom.fx", "PSMain_Combine");
		{
			D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			};
			auto rastDesc = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
			//rastDesc.DepthClipEnable = FALSE;
			//CD3DX12_DEPTH_STENCIL_DESC 
			//パイプラインステートを作成。
			D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = { 0 };
			psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
			psoDesc.pRootSignature = m_rootSign.Get();
			psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsBloom.GetCompiledBlob());
			psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psLuminance.GetCompiledBlob());
			psoDesc.RasterizerState = rastDesc;
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
			m_pipeLuminance.Init(psoDesc);

			psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psCombine.GetCompiledBlob());
			m_pipeCombine.Init(psoDesc);
		}

		m_descLuminance.RegistShaderResource(0, g_graphicsEngine->GetCurrentRenderTarget()->GetRenderTargetTexture());
		
		
		m_descLuminance.Commit();

		for (int i = 0; i < MAX_GAUSS; i++)
		{
			m_descCombine.RegistShaderResource(i, *m_blur[i].GetOutputTexture());
		}
		m_descCombine.Commit();

		m_copy.Init(currentRT->GetRenderTargetTexture(), TextureCopy::BlendMethod::BM_Add);
	}
	void Bloom::Render(RenderContext& rc)
	{
		const auto& prim = m_postEffect->GetPrimitive();
		rc.WaitUntilFinishDrawingToRenderTarget(g_graphicsEngine->GetCurrentRenderTarget()->GetRenderTargetTexture().Get());
		rc.WaitUntilToPossibleSetRenderTarget(m_luminanceRT);
		rc.SetRenderTarget(m_luminanceRT.GetRTVCpuDescriptorHandle(), m_luminanceRT.GetDSVCpuDescriptorHandle());
		D3D12_VIEWPORT viewport;
		viewport.Height = m_luminanceRT.GetHeight();
		viewport.Width = m_luminanceRT.GetWidth();
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.MinDepth = 0;
		viewport.MaxDepth = 1;
		rc.SetViewport(viewport);
		rc.SetRootSignature(m_rootSign);
		rc.SetDescriptorHeap(m_descLuminance);
		rc.SetPipelineState(m_pipeLuminance);
		prim.Draw(rc);

		rc.WaitUntilFinishDrawingToRenderTarget(m_luminanceRT);
		for (int i = 0; i < MAX_GAUSS; i++)
		{
			m_blur[i].Render(rc, prim);
		}

		rc.WaitUntilToPossibleSetRenderTarget(m_combineRT);
		rc.SetRenderTarget(m_combineRT.GetRTVCpuDescriptorHandle(), m_combineRT.GetDSVCpuDescriptorHandle());
		viewport.Height = m_combineRT.GetHeight();
		viewport.Width = m_combineRT.GetWidth();
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.MinDepth = 0;
		viewport.MaxDepth = 1;
		rc.SetViewport(viewport);
		rc.SetRootSignature(m_rootSign);
		rc.SetDescriptorHeap(m_descCombine);
		rc.SetPipelineState(m_pipeCombine);
		prim.Draw(rc);
		
	}
	void Bloom::CombineRender(RenderContext& rc, RenderTarget* rt)
	{	
		rc.WaitUntilFinishDrawingToRenderTarget(m_combineRT);
		m_copy.Render(rc, *rt, m_postEffect->GetPrimitive());
	}
}