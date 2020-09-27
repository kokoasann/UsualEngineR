#include "PreCompile.h"
#include "Physics/PhysicsDebugDraw.h"



namespace UER
{
	void PhysicsDebugDraw::Init()
	{
		m_primi.Cteate(D3D_PRIMITIVE_TOPOLOGY_LINELIST, MAX_VERTEX * sizeof(Vector4), sizeof(Vector4), &m_vertexBuffer[0], MAX_VERTEX, Primitive::it_4byte, &m_indexBuffer[0]);
		m_vs.LoadVS(L"Assets/shader/linePrimitive.fx", "VSMain");
		m_ps.LoadPS(L"Assets/shader/linePrimitive.fx", "PSMain");


		m_rootSign.Init(
			D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP);


		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },

		};

		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = { 0 };
		psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
		psoDesc.pRootSignature = m_rootSign.Get();
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vs.GetCompiledBlob());
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_ps.GetCompiledBlob());
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psoDesc.DepthStencilState.DepthEnable = TRUE;
		psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		psoDesc.DepthStencilState.StencilEnable = FALSE;
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;		//アルベドカラー出力用。s
		psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		psoDesc.SampleDesc.Count = 1;

		m_pipState.Init(psoDesc);

		m_constbuff.Init(sizeof(SConstantBuffer),nullptr);

		m_decsHeap.RegistConstantBuffer(0, m_constbuff);
		m_decsHeap.Commit();
	}

	void PhysicsDebugDraw::BeginRender()
	{
		m_numLine = 0;
	}

	void PhysicsDebugDraw::EndRender()
	{
		SConstantBuffer cb;
		cb.mView = g_camera3D->GetViewMatrix();
		cb.mProj = g_camera3D->GetProjectionMatrix();
		
		auto& rc = g_graphicsEngine->GetRenderContext();

		
		m_primi.GetIndexBuffer().Copy(&m_indexBuffer[0]);
		m_primi.GetVertexBuffer().Copy(&m_vertexBuffer[0]);

		m_constbuff.CopyToVRAM(&cb);
		rc.SetDescriptorHeap(m_decsHeap);
		rc.SetPipelineState(m_pipState);


		m_primi.Draw(rc, m_numLine * 2);
	}

	void PhysicsDebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
	{
		int base = m_numLine * 2;
		m_vertexBuffer[base].Set(Vector3{ from.x(),from.y(),from.z() });
		m_vertexBuffer[base + 1].Set(Vector3{ to.x(),to.y(),to.z() });
		m_indexBuffer[base] = base;
		m_indexBuffer[base+1] = base+1;
		m_numLine++;
	}

}