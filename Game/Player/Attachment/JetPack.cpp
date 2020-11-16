#include "stdafx.h"
#include "JetPack.h"
#include "../../Enemy/EnemyManager.h"
#include "JetEffect.h"

JetPack::JetPack()
{

}

JetPack::~JetPack()
{

}



void JetPack::Release()
{
	DeleteGO(m_model);
}

void JetPack::OnDestroy()
{
	Release();
}


void JetPack::Awake()
{

}

bool JetPack::Start()
{

	ModelInitData mid;
	m_model = NewGO<ModelRender>(0);
	mid.m_tkmFilePath = "Assets/modelData/m/Backpack/Backpack.tkm";
	mid.m_tksFilePath = "Assets/modelData/m/Backpack/Backpack.tks";
	//mid.m_upAxis = enUpAxisZ;
	m_model->Init(mid);

	/*
	PlaneParticleEffectInitData pid;
	pid.m_ddsFilePath = L"Assets/Image/illumination.dds";
	pid.m_height = 10;
	pid.m_width = 10;
	pid.m_extendDataSize = sizeof(float);
	pid.m_isBillboard = true;
	//pid.m_isBillboard = false;
	//pid.m_isDepthTest = false;
	PlaneParticleUpdater m_effctUpdater(
		[&]PLANE_PARTICLE_GENERATE_FUNC(pThis, deltaTime)
	{
		static float time = 0;
		auto pThrust = EnemyManager::GetEnemyManager().GetPlayer()->IsUsingThrusters();

		if (pThrust)
		{
			//Matrix m = g_matIdentity;
			//pThis->AddParticle(m, { 1,1,1,1 }, 10);
			for (int _i = 0; _i < 100; _i++)
			{
				float i = GRandom().Rand();
				auto posDif = Vector3::Zero;
				posDif.y += 1000.f * i;
				pThis->AddParticle(posDif, g_vec3One * 20, g_quatIdentity, { 3,2.f,0.3,1 }, 0.5, &i, true);
			}
			time = 0;
		}
		time += deltaTime;
	},
		[&]PLANE_PARTICLE_UPDATE_FUNC(data, deltaTime, extendData)
	{
		auto s = *(float*)extendData;
		data.particleData.pos.y += 500.f * deltaTime;

		float n = GPerlinNoise2D().GenerateNoise({ s * 10, data.particleData.pos.y / 10.f });
		float m = GPerlinNoise2D().GenerateNoise({ data.particleData.pos.y / 10.f, s * 10 });
		data.particleData.pos.x = n * 500.f * deltaTime;
		data.particleData.pos.z = m * 500.f * deltaTime;
		data.particleData.sca = g_vec3One * min((data.lifeTime / 10.f) + 0.1f, 1.f) * 20;

		Vector3 col;
		col.Lerp(data.lifeTime / 10.f, { 3,0.1f,0.0 }, { 3,1.5f,0.3 });
		data.particleData.mulColor.Set(col);
		data.particleData.mulColor.a = data.lifeTime / 10.f;

	});


	pid.m_updater = &m_effctUpdater;

	auto effect = NewGO<PlaneParticleEffectRender>(0);
	effect->Init(pid);
	effect->SetPos({ 0,0,50 });
	effect->SetSca(g_vec3One * 0.02);
	*/

	JetEffect::JetEffectInitParam jeip;
	jeip.effectScale = 0.01f;
	jeip.effectScale_inv = 100.f;
	jeip.particleScale = 500.f;
	jeip.particleLifeTime = 0.5f;
	jeip.particleYUp = 400.f;

	const int numEffect = 6;
	for (int i = 0; i < numEffect; i++) {
		auto ef = NewGO<JetEffect>(0);
		ef->Init(jeip);
		m_jetEffects.push_back(ef);
	}

	m_backpackBone[0] = m_model->GetModel().GetSkelton()->GetBone(m_model->GetModel().GetSkelton()->FindBoneID(L"Pack.001"));
	m_backpackBone[5] = m_model->GetModel().GetSkelton()->GetBone(m_model->GetModel().GetSkelton()->FindBoneID(L"Pack.002"));
	m_backpackBone[1] = m_model->GetModel().GetSkelton()->GetBone(m_model->GetModel().GetSkelton()->FindBoneID(L"Bombe_L.001"));
	m_backpackBone[2] = m_model->GetModel().GetSkelton()->GetBone(m_model->GetModel().GetSkelton()->FindBoneID(L"Bombe_L.002"));
	m_backpackBone[3] = m_model->GetModel().GetSkelton()->GetBone(m_model->GetModel().GetSkelton()->FindBoneID(L"Bombe_R.001"));
	m_backpackBone[4] = m_model->GetModel().GetSkelton()->GetBone(m_model->GetModel().GetSkelton()->FindBoneID(L"Bombe_R.002"));

	return true;
}


void JetPack::PreUpdate()
{

}

void JetPack::Update()
{

	m_model->SetPosition(m_position);
	m_model->SetRotation(m_rotation);
	m_model->SetScale(m_scale);

	if (!m_isUsed) {
		m_model->SetActive(false);
		for (int i = 0; i < m_jetEffects.size(); i++) {
			m_jetEffects[i]->SetGenerateFlag(false);
		}
	}
	else {
		m_model->SetActive(true);
	}

}

void JetPack::PostUpdate()
{

	if (!m_isUsed) return;

	if (!m_isUsingThrusters) {
		for (int i = 0; i < m_jetEffects.size(); i++) {
			m_jetEffects[i]->SetGenerateFlag(false);
		}
		return;
	}

	for (int i = 0; i < m_jetEffects.size(); i++) {
		m_jetEffects[i]->SetGenerateFlag(true);
		const auto& mat = m_backpackBone[i]->GetWorldMatrix();
		m_jetEffects[i]->SetPosition(mat.GetTransrate());
		m_jetEffects[i]->SetRotation(mat.GetRotate());
	}

}


void JetPack::Render()
{

}

void JetPack::PostRender()
{

}
