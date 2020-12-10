#include "PreCompile.h"
#include "PreRender.h"
#include "BufferEnum.h"



namespace UER
{
	void PreRender::Init()
	{
		m_gBuffer.Init();

		m_deferredCB.Init(sizeof(DeferredCB));
		
		m_vs.LoadVS(L"Assets/shader/Deferred.fx", "VSMain");
		m_ps.LoadPS(L"Assets/shader/Deferred.fx", "PSMain");

		m_rootSign.Init(
			D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
			8, 11, 1);

		// 頂点レイアウトを定義する。
		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};

		//パイプラインステートを作成。
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = { 0 };
		psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
		psoDesc.pRootSignature = m_rootSign.Get();
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vs.GetCompiledBlob());
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_ps.GetCompiledBlob());
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psoDesc.DepthStencilState.DepthEnable = FALSE;
		psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		psoDesc.DepthStencilState.StencilEnable = FALSE;
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = TO_INT(EGBufferKind::NumKind);
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;		//アルベドカラー出力用。
		psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		psoDesc.SampleDesc.Count = 1;
		m_pipState.Init(psoDesc);


		auto gbuffs = m_gBuffer.GetGBuffer();
		for(int i=0;i<TO_INT(EGBufferKind::NumKind);i++)
		{
			m_descHeap.RegistShaderResource(i, gbuffs[i].GetRenderTargetTexture());
		}
		m_descHeap.RegistConstantBuffer(0, m_deferredCB);
		auto& ligMana = g_graphicsEngine->GetLightManager();

		m_descHeap.RegistConstantBuffer(TO_INT(EConstantBuffer::cb_lightData),ligMana.GetLightConstBuffer());
		m_descHeap.RegistShaderResource(TO_INT(ETextureBuffer::tb_dirLight), ligMana.GetDirLightStructuredBuffer());
		m_descHeap.RegistShaderResource(TO_INT(ETextureBuffer::tb_pointLight), ligMana.GetPointLightStructuredBuffer());

		m_descHeap.RegistConstantBuffer(TO_INT(EConstantBuffer::cb_shadowData), g_graphicsEngine->GetShadowMap().GetShadowCB());
		m_descHeap.RegistShaderResource(TO_INT(ETextureBuffer::tb_shadow1), g_graphicsEngine->GetShadowMap().GetShadowMap(0).GetRenderTargetTexture());
		m_descHeap.RegistShaderResource(TO_INT(ETextureBuffer::tb_shadow2), g_graphicsEngine->GetShadowMap().GetShadowMap(1).GetRenderTargetTexture());
		m_descHeap.RegistShaderResource(TO_INT(ETextureBuffer::tb_shadow3), g_graphicsEngine->GetShadowMap().GetShadowMap(2).GetRenderTargetTexture());

		m_descHeap.Commit();
	}
	void PreRender::Render(RenderContext& rc)
	{
		m_gBuffer.Draw(rc);
	}
	void PreRender::LightingRender(RenderContext& rc)
	{
		DeferredCB dcb;
		dcb.mVPI = g_camera3D->GetViewProjectionMatrix();
		dcb.mVPI.Inverse();
		dcb.camPos = g_camera3D->GetPosition();
		dcb.camDir = g_camera3D->GetForward();
		dcb.camNear = g_camera3D->GetNear();
		dcb.camFar = g_camera3D->GetFar();
		m_deferredCB.CopyToVRAM(dcb);

		//rc.WaitUntilFinishDrawingToRenderTargets(TO_INT(EGBufferKind::NumKind), m_gBuffer.GetGBuffer());

		auto mainRT = g_graphicsEngine->GetCurrentRenderTarget();
		
		rc.WaitUntilFinishDrawingToRenderTarget(*mainRT);
		rc.WaitUntilToPossibleSetRenderTarget(*mainRT);
		rc.SetRenderTargets(1, mainRT);
		
		
		rc.SetRootSignature(m_rootSign);
		rc.SetDescriptorHeap(m_descHeap);
		rc.SetPipelineState(m_pipState);

		const auto& pri = g_graphicsEngine->GetPrimitive();
		pri.Draw(rc);

		//rc.WaitUntilFinishDrawingToRenderTarget(*mainRT);
	}
}