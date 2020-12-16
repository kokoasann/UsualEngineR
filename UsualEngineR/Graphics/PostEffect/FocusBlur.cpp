#include "PreCompile.h"
#include "FocusBlur.h"


namespace UER
{
	void FocusBlur::Init()
	{
		m_constBuff.Init(sizeof(BlurData));
		float cc[] = { 1,1,1,0 };
		m_rt.Create(FRAME_BUFFER_W, FRAME_BUFFER_H, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D16_UNORM, cc);

		m_rootSign.Init(
			D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP
		);

		m_descHeap.RegistConstantBuffer(0, m_constBuff);
		m_descHeap.RegistShaderResource(0, g_graphicsEngine->GetCurrentRenderTarget()->GetRenderTargetTexture());
		m_descHeap.Commit();

		m_vs.LoadVS(L"Assets/shader/FocusBlur.fx", "VSMain");
		m_ps.LoadPS(L"Assets/shader/FocusBlur.fx", "PSMain");


		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};
		CD3DX12_RASTERIZER_DESC rastDesc(D3D12_DEFAULT);
		D3D12_BLEND_DESC blendDesc;
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		D3D12_RENDER_TARGET_BLEND_DESC rtblenddesc;
		rtblenddesc =
		{
			FALSE,FALSE,
			D3D12_BLEND_ONE,D3D12_BLEND_ONE,D3D12_BLEND_OP_ADD,
			D3D12_BLEND_ONE,D3D12_BLEND_ONE,D3D12_BLEND_OP_ADD,
			D3D12_LOGIC_OP_NOOP,
			D3D12_COLOR_WRITE_ENABLE_ALL
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
		m_pipState.Init(psoDesc);

		Texture* texs[] = { &m_rt.GetRenderTargetTexture() };
		m_texCopy.Init(texs, TextureCopy::BlendMethod::BM_Trans);
	}

	void FocusBlur::Render(RenderContext& rc,const Primitive& prim)
	{
		m_blurData.time += gameTime()->GetDeltaTime();
		m_constBuff.CopyToVRAM(m_blurData);

		auto mainRT = g_graphicsEngine->GetCurrentRenderTarget();
		rc.WaitUntilFinishDrawingToRenderTarget(*mainRT);
		rc.WaitUntilToPossibleSetRenderTarget(m_rt);

		D3D12_VIEWPORT vp{ 0,0,FRAME_BUFFER_W,FRAME_BUFFER_H,0,1 };
		rc.SetViewport(vp);
		
		RenderTarget* rts[] = { &m_rt };
		rc.SetRenderTargets(1, rts);

		rc.SetRootSignature(m_rootSign);
		rc.SetDescriptorHeap(m_descHeap);
		rc.SetPipelineState(m_pipState);

		prim.Draw(rc);
		
		rc.WaitUntilFinishDrawingToRenderTarget(m_rt);
		rc.WaitUntilToPossibleSetRenderTarget(*mainRT);
		m_texCopy.Render(rc, *mainRT, prim);
	}
}