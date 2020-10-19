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
	void PlaneParticleEffect::Init(const PlaneParticleEffectInitData& ini)
	{
		m_texture.InitFromDDSFile(ini.m_ddsFilePath);
		if (ini.m_width == 0 || ini.m_height == 0)
		{
			m_width = m_texture.GetWidth();
			m_height = m_texture.GetHeight();
		}

		m_constBuffer.Init(sizeof(SConstBuffData));
		m_structuredBuff.Init(sizeof(Matrix), MAX_INSTANCES_NUM, nullptr);

		m_vs.LoadVS(L"Assets/shader/SpriteInstanceing.fx", "VSMain");
		if(!ini.m_isDepthTest)
			m_ps.LoadPS(L"Assets/shader/SpriteInstanceing.fx", "PSMain");
		else
			m_ps.LoadPS(L"Assets/shader/SpriteInstanceing.fx", "PSMain_DepthEnable");
		
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
	}
	void PlaneParticleEffect::Update(float deltaTime, const Vector3& pos, const Quaternion& rot, const Vector3& sca)
	{
		for (auto& par : m_particleDatas)
		{
			//m_updateFunc(par);
		}

		std::list<int> deths;
		for (int i=0;i<m_numInstance;i++)
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
			for (int j = i; j < m_numInstance; i++)
			{
				m_particleDatas[j] = m_particleDatas[j + 1];
				m_particleTimes[j] = m_particleTimes[j + 1];
			}
			m_numInstance--;
		}


	}
	void PlaneParticleEffect::Draw(RenderContext& rc)
	{
		m_structuredBuff.Update(&m_particleDatas[0]);

	}
}