#include "stdafx.h"
#include "JetPack.h"


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
		if (time >= 0.01f)
		{
			//Matrix m = g_matIdentity;
			//pThis->AddParticle(m, { 1,1,1,1 }, 10);
			for (int _i = 0; _i < 3; _i++)
			{
				float i = GRandom().Rand();
				pThis->AddParticle(g_vec3Zero, g_vec3One, g_quatIdentity, { 3,2.f,0.3,1 }, 10, &i);
			}
			time = 0;
		}
		time += deltaTime;
	},
		[&]PLANE_PARTICLE_UPDATE_FUNC(data, deltaTime, extendData)
	{
		auto s = *(float*)extendData;
		data.particleData.pos.y += 30.f * deltaTime;

		float n = GPerlinNoise2D().GenerateNoise({ s * 10, data.particleData.pos.y / 10.f });
		float m = GPerlinNoise2D().GenerateNoise({ data.particleData.pos.y / 10.f, s * 10 });
		data.particleData.pos.x = n * 500.f * deltaTime;
		data.particleData.pos.z = m * 500.f * deltaTime;
		data.particleData.sca = g_vec3One * min((data.lifeTime / 10.f) + 0.1f, 1.f);

		Vector3 col;
		col.Lerp(data.lifeTime / 10.f, { 3,0.1f,0.0 }, { 3,1.5f,0.3 });
		data.particleData.mulColor.Set(col);
		data.particleData.mulColor.a = data.lifeTime / 10.f;

	});


	pid.m_updater = &m_effctUpdater;

	m_effect = NewGO<PlaneParticleEffectRender>(0);
	m_effect->Init(pid);
	m_effect->SetPos({ 0,0,50 });
	m_effect->SetSca(g_vec3One * 0.02);
	m_effect_2 = NewGO<PlaneParticleEffectRender>(0);
	m_effect_2->Init(pid);
	m_effect_2->SetPos({ 0,0,50 });
	m_effect_2->SetSca(g_vec3One * 0.02);

	for (int i = 0; i < 4; i++)
	{
		m_effects[i] = NewGO<PlaneParticleEffectRender>(0);
		m_effects[i]->Init(pid);
		m_effects[i]->SetSca(g_vec3One * 0.02);
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
	if (!m_isUsed) {
		m_model->SetActive(false);
		for (int i = 0; i < 4; i++)
		{
			m_effects[i]->SetActive(false);
		}
		m_effect->SetActive(false);
		m_effect_2->SetActive(false);
		return;
	}

	m_model->SetActive(true);

	m_model->SetPosition(m_position);
	m_model->SetRotation(m_rotation);
	m_model->SetScale(m_scale);
}

void JetPack::PostUpdate()
{
	if (!m_isUsed) {
		for (int i = 0; i < 4; i++)
		{
			m_effects[i]->SetActive(false);
		}
		m_effect_2->SetActive(false);
		m_effect->SetActive(false);
	}
	else {
		for (int i = 0; i < 4; i++)
		{
			m_effects[i]->SetActive(true);
			const auto& mat = m_backpackBone[i + 1]->GetWorldMatrix();
			m_effects[i]->SetRot(mat.GetRotate());
			m_effects[i]->SetPos(mat.GetTransrate());
		}

		m_effect->SetActive(true);
		m_effect_2->SetActive(true);


		{
			const auto& mat = m_backpackBone[0]->GetWorldMatrix();
			m_effect->SetRot(mat.GetRotate());
			m_effect->SetPos(mat.GetTransrate());
		}
		{
			const auto& mat = m_backpackBone[5]->GetWorldMatrix();
			m_effect_2->SetRot(mat.GetRotate());
			m_effect_2->SetPos(mat.GetTransrate());
		}
	}
}


void JetPack::Render()
{

}

void JetPack::PostRender()
{

}
