#include "PreCompile.h"
#include "VolumetricEffect.h"



namespace UER
{
	void AnchorModel::Init(const char* tkmFile, RWStructuredBuffer& vertPosList)
	{
		m_tkm.Load(tkmFile);

		int count = 0;
		m_tkm.QueryMeshParts([&](const TkmFile::SMesh& mesh)
			{
				if (count > 0)
				{
#if DEBUG_FUNC
					MessageBoxA(nullptr, "one skin model only!!! sory...", "error", MB_OK);
					std::abort();
#else
					return;
#endif
				}

				int vertNum = mesh.vertexBuffer.size();
				int vertSize = sizeof(TkmFile::SVertex);
				m_vertBuffer.Init(vertSize*vertNum, vertSize);
				m_vertBuffer.Copy((void*)&mesh.vertexBuffer[0]);

				m_vertBuffer_inv.Init(vertSize * vertNum, vertSize);
				m_vertBuffer_inv.Copy((void*)&mesh.vertexBuffer[0]);
				auto vdata = reinterpret_cast<TkmFile::SVertex*>(m_vertBuffer_inv.BeginEgit());

				float top = -FLT_MAX;
				float bottom = FLT_MAX;
				Vector3 vertSum = g_vec3Zero;
				for (int i = 0; i < vertNum; i++)
				{
					vdata[i].normal *= -1.f;
					vertSum += vdata[i].pos;

					top = max(vdata[i].pos.y, top);
					bottom = min(vdata[i].pos.y, bottom);
				}
				m_vertBuffer_inv.EndEdit();

				m_center = vertSum / vertNum;
				m_centerBottom.Set(m_center.x, bottom, m_center.z);
				m_centerTop.Set(m_center.x, top, m_center.z);
				m_centerAxis = m_centerTop - m_centerBottom;
				

				if (!mesh.indexBuffer16Array.empty())
				{
					m_indBuffer.Init(mesh.indexBuffer16Array[0].indices.size() * 2, 2);
					m_indBuffer.Copy((void*)&mesh.indexBuffer16Array[0].indices[0]);

					m_indBuffer_inv.Init(mesh.indexBuffer16Array[0].indices.size() * 2, 2);
					m_indBuffer_inv.Copy((void*)&mesh.indexBuffer16Array[0].indices[0]);
					auto idata = reinterpret_cast<uint16_t*>(m_indBuffer_inv.BeginEgit());
					for (int i = 0; i < mesh.indexBuffer16Array[0].indices.size() / 3; i++)
					{
						int b = i * 3;
						std::swap(idata[b], idata[b + 2]);
					}
					m_indBuffer_inv.EndEdit();
				}
				else
				{
					m_indBuffer.Init(mesh.indexBuffer32Array[0].indices.size() * 4, 4);
					m_indBuffer.Copy((void*)&mesh.indexBuffer32Array[0].indices[0]);
				}

				count++;
			});

		m_constBuffer.Init(sizeof(SConstBufferData));

		vertPosList.Init(sizeof(Vector3), GetVertexNum(), nullptr);

		m_vs.LoadVS(L"Assets/shader/AnchorModel.fx", "VSMain");
		m_vs_inv.LoadVS(L"Assets/shader/AnchorModel.fx", "VSMain_inv");
		m_ps.LoadPS(L"Assets/shader/AnchorModel.fx", "PSMain");
		m_ps_inv.LoadPS(L"Assets/shader/AnchorModel.fx", "PSMain_inv");

		m_rootSign.Init(
			D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			1,1,1
		);

		// 頂点レイアウトを定義する。
		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};

		

		//パイプラインステートを作成。
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
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 2;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R32_FLOAT;		//アルベドカラー出力用。
		psoDesc.RTVFormats[1] = DXGI_FORMAT_R8_UINT;		//ステンシル出力用。
		psoDesc.DSVFormat = DXGI_FORMAT_D16_UNORM;
		psoDesc.SampleDesc.Count = 1;
		m_pipState.Init(psoDesc);

		psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vs_inv.GetCompiledBlob());
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[1] = DXGI_FORMAT_UNKNOWN;
		//m_pipState_inv.Init(psoDesc);


		m_descHeap.RegistConstantBuffer(0, m_constBuffer);
		m_descHeap.RegistUnorderAccessResource(0, vertPosList);
		m_descHeap.Commit();
	}
	void AnchorModel::Draw(RenderContext& rc, const Matrix& mwvp, bool isInv)
	{
		m_constBuffer.CopyToVRAM((void*)&mwvp);

		
		
		rc.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		rc.SetRootSignature(m_rootSign);
		if (!isInv)
		{
			rc.SetIndexBuffer(m_indBuffer);
			rc.SetVertexBuffer(m_vertBuffer);
			rc.SetPipelineState(m_pipState);
		}
		else
		{
			rc.SetIndexBuffer(m_indBuffer_inv);
			rc.SetVertexBuffer(m_vertBuffer_inv);
			rc.SetPipelineState(m_pipState);
		}
		
		rc.SetDescriptorHeap(m_descHeap);


		rc.DrawIndexed(m_indBuffer.GetCount());
	}

	
	RenderTarget VolumetricEffect::m_anchorRT;
	RenderTarget VolumetricEffect::m_anchorRT_inv;
	RenderTarget VolumetricEffect::m_stencilRT;
	bool VolumetricEffect::m_isInitRT = false;

	void VolumetricEffect::Release()
	{
	}

	void VolumetricEffect::Init(const char* tkmFile)
	{
		
		//m_vertPos.Init(sizeof(Vector2), m_anchorModel.GetVertexNum(), nullptr);
		m_anchorModel.Init(tkmFile,m_vertPos);

		if (!m_isInitRT)
		{
			float cc[4] = { FLT_EPSILON,0,0,1 };
			m_anchorRT.Create(FRAME_BUFFER_W, FRAME_BUFFER_H, 1, 1, DXGI_FORMAT_R32_FLOAT, DXGI_FORMAT_D16_UNORM, cc);
			m_anchorRT_inv.Create(FRAME_BUFFER_W, FRAME_BUFFER_H, 1, 1, DXGI_FORMAT_R32_FLOAT, DXGI_FORMAT_D16_UNORM, cc);
			cc[0] = 0;
			m_stencilRT.Create(FRAME_BUFFER_W, FRAME_BUFFER_H, 1, 1, DXGI_FORMAT_R8_UINT, DXGI_FORMAT_D16_UNORM, cc);
			m_isInitRT = true;
		}

		struct SSimpleVertex {
			Vector4 pos;
			Vector2 tex;
		};
		//四角形プリミティブを初期化。
		SSimpleVertex vertices[] =
		{
			{
				Vector4(-1.0f, -1.0f, 0.0f, 1.0f),
				Vector2(0.0f, 1.0f),
			},
			{
				Vector4(1.0f, -1.0f, 0.0f, 1.0f),
				Vector2(1.0f, 1.0f),
			},
			{
				Vector4(-1.0f, 1.0f, 0.0f, 1.0f),
				Vector2(0.0f, 0.0f)
			},
			{
				Vector4(1.0f, 1.0f, 0.0f, 1.0f),
				Vector2(1.0f, 0.0f)
			}

		};
		short indices[] = { 0,1,2,3 };
		m_prim.Cteate(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, 4, sizeof(SSimpleVertex), vertices, 4, Primitive::it_2byte, indices);

		m_constBuffer.Init(sizeof(SConstBufferData));

		m_vs.LoadVS(L"Assets/shader/VolumetricEffect.fx", "VSMain");
		m_ps.LoadPS(L"Assets/shader/VolumetricEffect.fx", "PSMain");

		m_rootSign.Init(
			D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			1, 4, 1
		);

		// 頂点レイアウトを定義する。
		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};

		D3D12_BLEND_DESC blendDesc = {};
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		const D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc =
		{
			TRUE,FALSE,
			D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_INV_SRC_ALPHA, D3D12_BLEND_OP_ADD,
			D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
			D3D12_LOGIC_OP_NOOP,
			D3D12_COLOR_WRITE_ENABLE_ALL,
		};
		for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
			blendDesc.RenderTarget[i] = defaultRenderTargetBlendDesc;

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
		psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		psoDesc.DepthStencilState.StencilEnable = FALSE;
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;		//アルベドカラー出力用。
		psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		psoDesc.SampleDesc.Count = 1;
		m_pipState.Init(psoDesc);


		m_descHeap.RegistShaderResource(0, m_anchorRT.GetRenderTargetTexture());
		m_descHeap.RegistShaderResource(1, m_anchorRT_inv.GetRenderTargetTexture());
		m_descHeap.RegistShaderResource(2, m_stencilRT.GetRenderTargetTexture());
		m_descHeap.RegistShaderResource(3, g_graphicsEngine->GetPreRender().GetGBuffer().GetGBuffer(EGBufferKind::Depth).GetRenderTargetTexture());
		m_descHeap.RegistConstantBuffer(0, m_constBuffer);

		m_descHeap.Commit();
	}

	struct SVertPosXYW
	{
		Vector2 pos;
		float w;
	};
	void VolumetricEffect::Draw(RenderContext& rc, const Matrix& mworld, const Matrix& mvp, const FogData& fogData)
	{
		rc.WaitUntilToPossibleSetRenderTarget(m_anchorRT);
		rc.WaitUntilToPossibleSetRenderTarget(m_anchorRT_inv);
		rc.WaitUntilToPossibleSetRenderTarget(m_stencilRT);

		m_anchorRT.Clear(rc);
		m_anchorRT_inv.Clear(rc);
		m_stencilRT.Clear(rc);

		Matrix mwvp;
		mwvp.Multiply(mworld, mvp);

		RenderTarget* rts[] = { &m_anchorRT,&m_stencilRT };
		rc.SetRenderTargets(2, rts);
		m_anchorModel.Draw(rc, mwvp, false);

		//rc.WaitUntilToPossibleSetRenderTarget(m_stencilRT);

		//rc.SetRenderTargets(1, &m_anchorRT_inv);
		rts[0] = &m_anchorRT_inv;
		rc.SetRenderTargets(2, rts);
		m_anchorModel.Draw(rc, mwvp, true);

		
		rc.WaitUntilFinishDrawingToRenderTarget(m_anchorRT);
		rc.WaitUntilFinishDrawingToRenderTarget(m_anchorRT_inv);
		rc.WaitUntilFinishDrawingToRenderTarget(m_stencilRT);


		SVertPosXYW* vertPosList = reinterpret_cast<SVertPosXYW*>(m_vertPos.GetResourceOnCPU());
		Vector2 maxPos = { -FLT_MAX, -FLT_MAX }, minPos = { FLT_MAX, FLT_MAX };
		for (int i = 0; i < m_anchorModel.GetVertexNum(); i++)
		{
			vertPosList[i].pos.Scale(1.f/vertPosList[i].w);
			vertPosList[i].pos.x += 1.f;
			vertPosList[i].pos.y -= 1.f;
			vertPosList[i].pos.x /= 2.f;
			vertPosList[i].pos.y /= -2.f;
			vertPosList[i].pos.x *= FRAME_BUFFER_W;
			vertPosList[i].pos.y *= FRAME_BUFFER_H;

			maxPos.Max(vertPosList[i].pos);
			minPos.Min(vertPosList[i].pos);
		}
		maxPos.Min({FRAME_BUFFER_W,FRAME_BUFFER_H });
		minPos.Max({ 0,0 });
		D3D12_RECT rect;
		rect.left = minPos.x;
		rect.top = minPos.y;
		rect.right = maxPos.x;
		rect.bottom = maxPos.y;

		if (rect.left == 0 && rect.top == 0 && rect.right == 0 && rect.bottom == 0)
		{
			//return;
		}

		/*D3D12_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = static_cast<FLOAT>(FRAME_BUFFER_W);
		viewport.Height = static_cast<FLOAT>(FRAME_BUFFER_H);
		viewport.MinDepth = D3D12_MIN_DEPTH;
		viewport.MaxDepth = D3D12_MAX_DEPTH;*/
		

		SConstBufferData cbData;
		cbData.mvp = mvp;
		cbData.mvp_inv.Inverse(mvp);

		cbData.data.rayCount = fogData.rayCount;
		cbData.data.rayLen = fogData.rayLen;
		cbData.data.color = fogData.color;
		cbData.data.decayCenterToXZ = fogData.decayCenterToXZ;
		cbData.data.decayCenterToY = fogData.decayCenterToY;
		cbData.data.offset = fogData.offset;
		cbData.data.perlinScale = fogData.perlinScale;
		cbData.data.worleyScale = fogData.worleyScale;
		cbData.data.ratioParlinWorley = fogData.ratioParlinWorley;
		cbData.data.concentration = fogData.concentration;
		cbData.data.centerPos = m_anchorModel.GetCenter();
		//cbData.data.centerAxis = m_anchorModel.GetCenterAxis();
		//mworld.Apply(cbData.data.centerAxis);
		cbData.data.centerBottom = m_anchorModel.GetCenterBottom();
		mworld.Apply(cbData.data.centerBottom);
		cbData.data.centerTop = m_anchorModel.GetCenterTop();
		mworld.Apply(cbData.data.centerTop);

		m_constBuffer.CopyToVRAM(cbData);

		rc.SetRenderTargets(1, g_graphicsEngine->GetCurrentRenderTarget());

		//rc.SetScissorRect(rect);
		//rc.SetViewport(viewport);

		rc.SetRootSignature(m_rootSign);
		rc.SetPipelineState(m_pipState);
		rc.SetDescriptorHeap(m_descHeap);

		m_prim.Draw(rc);

		rect.left = 0;
		rect.top = 0;
		rect.right = FRAME_BUFFER_W;
		rect.bottom = FRAME_BUFFER_H;
		rc.SetScissorRect(rect);
	}



	void VolumetricEffectRender::Release()
	{
	}
	void VolumetricEffectRender::OnDestroy()
	{
	}
	void VolumetricEffectRender::Init(const char* tkmFile)
	{
		m_volumeEffect.Init(tkmFile);
	}
	void VolumetricEffectRender::Update()
	{
	}
	void VolumetricEffectRender::PrePostRender()
	{
		auto& rc = g_graphicsEngine->GetRenderContext();
		auto& mvp = g_camera3D->GetViewProjectionMatrix();

		Matrix tra, sca, rot, wmat;
		tra.MakeTranslation(m_pos);
		sca.MakeScaling(m_sca);
		rot.MakeRotationFromQuaternion(m_rot);
		wmat.Multiply(sca, rot);
		wmat.Multiply(wmat, tra);

		m_volumeEffect.Draw(rc, wmat, mvp, m_fogData);
	}
}