#include "PreCompile.h"
#include "ParticleEffect.h"



namespace UER
{

	namespace {
		struct SSimpleVertex {
			Vector4 pos;
			Vector2 tex;
		};
	}
	void PlaneParticleEffect::Release()
	{
	}
	void PlaneParticleEffect::Init(const PlaneParticleEffectInitData& ini)
	{
		//m_updater = ini.m_updater;
		m_updateFunc = ini.m_updater->m_updateFunc;
		m_generateFunc = ini.m_updater->m_geneFunc;

		m_texture.InitFromDDSFile(ini.m_ddsFilePath);
		if (ini.m_width == 0 || ini.m_height == 0)
		{
			m_width = m_texture.GetWidth();
			m_height = m_texture.GetHeight();
		}
		else
		{
			m_width = ini.m_width;
			m_height = ini.m_height;
		}

		m_constBuffer.Init(sizeof(SConstBuffData));
		m_structuredBuff.Init(sizeof(Matrix), MAX_INSTANCES_NUM, nullptr);

		m_vs.LoadVS(L"Assets/shader/SpriteInstancing.fx", "VSMain");
		if(!ini.m_isDepthTest)
			m_ps.LoadPS(L"Assets/shader/SpriteInstancing.fx", "PSMain");
		else
			m_ps.LoadPS(L"Assets/shader/SpriteInstancing.fx", "PSMain_DepthEnable");
		
		float halfW = m_width * 0.5f;
		float halfH = m_height * 0.5f;
		//頂点バッファのソースデータ。
		SSimpleVertex vertices[] =
		{
			{
				Vector4(-halfW, -halfH, 0.0f, 1.0f),
				Vector2(1.0f, 1.0f),
			},
			{
				Vector4(halfW, -halfH, 0.0f, 1.0f),
				Vector2(0.0f, 1.0f),
			},
			{
				Vector4(-halfW, halfH, 0.0f, 1.0f),
				Vector2(1.0f, 0.0f)
			},
			{
				Vector4(halfW, halfH, 0.0f, 1.0f),
				Vector2(0.0f, 0.0f)
			}

		};
		short indices[] = { 0,1,2,3 };

		m_vertBuff.Init(sizeof(SSimpleVertex) * 4, sizeof(SSimpleVertex));
		m_vertBuff.Copy(vertices);

		m_indexBuff.Init(sizeof(short) * 4, sizeof(short));
		m_indexBuff.Copy(indices);

		m_rootSign.Init(
			D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			1,3,1
		);
		
		// 頂点レイアウトを定義する。
		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
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
		psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		psoDesc.DepthStencilState.StencilEnable = FALSE;
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		psoDesc.DSVFormat = DXGI_FORMAT_D16_UNORM;
		psoDesc.SampleDesc.Count = 1;
		m_pipState.Init(psoDesc);

		m_descHeap.RegistConstantBuffer(0, m_constBuffer);
		m_descHeap.RegistShaderResource(0, m_texture);
		m_descHeap.RegistShaderResource(1, m_structuredBuff);
		if (ini.m_isDepthTest)
		{
			auto& depth = g_graphicsEngine->GetPreRender().GetGBuffer().GetGBuffer(EGBufferKind::Depth);
			m_descHeap.RegistShaderResource(2, depth.GetRenderTargetTexture());
		}
		m_descHeap.Commit();
	}
	void PlaneParticleEffect::Update(float deltaTime)
	{
		//寿命切れの奴を消す。
		std::list<int> deths;
		for (int i = 0; i < m_numInstance; i++)
		{
			m_particleTimes[i] -= deltaTime;
			if (m_particleTimes[i] <= 0)
			{
				deths.push_back(i);
			}
		}
		deths.reverse();
		for (int i : deths)
		{
			/*for (int j = i; j < m_numInstance; j++)
			{
				m_particleDatas[j] = m_particleDatas[j + 1];
				m_particleTimes[j] = m_particleTimes[j + 1];
			}*/


			if (i != m_particleDatas.size() - 1)
			{
				std::swap(m_particleDatas[i], m_particleDatas[m_particleDatas.size() - 1]);
				m_particleDatas.pop_back();

				std::swap(m_particleTimes[i], m_particleTimes[m_particleTimes.size() - 1]);
				m_particleTimes.pop_back();
			}
			else
			{
				m_particleDatas.pop_back();
				m_particleTimes.pop_back();
			}

			m_numInstance--;
		}

		//生み出す。
		//m_updater->m_geneFunc(this, deltaTime);
		m_generateFunc(this, deltaTime);

		//更新。
		for (int i =0;i < m_numInstance;i++)
		{
			//m_updater->m_updateFunc(m_particleDatas[i]);
			m_updateFunc(m_particleDatas[i],deltaTime);
		}
	}
	void PlaneParticleEffect::Draw(
		RenderContext& rc,
		const Vector3& pos,
		const Vector3& sca,
		const Quaternion& rot,
		const Vector4& mulColor,
		const Matrix& view,
		const Matrix& projection
	)
	{
		if (m_numInstance == 0)
			return;
		Matrix vrot = view;
		vrot.SetTranspose({ 0,0,0 });
		vrot.Inverse();
		Matrix mTra, mSca, mRot, mWor, mvp;
		mSca.MakeScaling(sca);
		mRot.MakeRotationFromQuaternion(rot);
		mRot.Multiply(vrot, mRot);
		mTra.MakeTranslation(pos);
		mWor.Multiply(mSca, mRot);
		mWor.Multiply(mWor, mTra);

		mvp.Multiply(mWor, view);
		mvp.Multiply(mvp, projection);

		SConstBuffData data(mvp, mulColor);

		SParticleData datas[MAX_INSTANCES_NUM];
		for (int i = 0; i < m_numInstance; i++)
		{
			datas[i] = m_particleDatas[i];
		}
		m_structuredBuff.Update(datas, m_numInstance);
		m_constBuffer.CopyToVRAM(data);

		rc.SetIndexBuffer(m_indexBuff);
		rc.SetVertexBuffer(m_vertBuff);

		rc.SetRootSignature(m_rootSign);
		rc.SetPipelineState(m_pipState);
		rc.SetDescriptorHeap(m_descHeap);

		rc.DrawIndexedInstanced(4, m_numInstance, 0, 0, 0);
	}
	void PlaneParticleEffect::AddParticle(const Matrix& mw, const Vector4& mulColor, float lifeTime)
	{
		m_particleDatas.resize(m_particleDatas.size() + 1);
		m_particleDatas[m_particleDatas.size()-1].mWorld = mw;
		m_particleDatas[m_particleDatas.size()-1].mulColor = mulColor;

		m_particleTimes.resize(m_particleTimes.size() + 1);
		m_particleTimes[m_particleTimes.size()-1] = lifeTime;
		m_numInstance++;

		
	}





	void PlaneParticleEffectRender::Init(const PlaneParticleEffectInitData& pid)
	{
		m_effect.Init(pid);
	}

	void PlaneParticleEffectRender::Update()
	{
		float dtime = gameTime()->GetDeltaTime();
		m_effect.Update(dtime);
	}

	void PlaneParticleEffectRender::PrePostRender()
	{
		auto& rc = g_graphicsEngine->GetRenderContext();
		const auto& view = g_camera3D->GetViewMatrix();
		const auto& proj = g_camera3D->GetProjectionMatrix();
		m_effect.Draw(rc, m_pos, m_sca, m_rot, m_mulColor, view, proj);
	}
}