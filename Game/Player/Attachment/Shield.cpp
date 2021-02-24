#include "stdafx.h"
#include "Shield.h"
#include "Effect/SmokeEffect.h"
#include "GameManager.h"
#include "GameSceneMenu.h"

Shield::Shield()
{

}

Shield::~Shield()
{

}



void Shield::Release()
{
	DeleteGO(m_model);
	for (int i = 0; i < m_worldSmokeEffects.size(); i++) {
		DeleteGO(m_worldSmokeEffects.at(i));
	}
	//for (int i = 0; i < m_localSmokeEffects.size(); i++) {
		//DeleteGO(m_localSmokeEffects.at(i));
	//}
}

void Shield::OnDestroy()
{
	Release();
}


void Shield::Awake()
{

}

bool Shield::Start()
{

	ModelInitData mid;
	m_model = NewGO<ModelRender>(0);
	mid.m_tkmFilePath = "Assets/modelData/m/Shield/Shield.tkm";
	mid.m_tksFilePath = "Assets/modelData/m/Shield/Shield.tks";
	//mid.m_upAxis = enUpAxisZ;
	m_model->Init(mid);

	const int numEffect = 3;

	for (int i = 0; i < numEffect; i++) {
		auto ef = NewGO<SmokeEffect>(0);
		ef->Init(Vector4(1.f,1.f,1.f,1.f),Vector4(1.f,1.f,1.f,0.f), /*lifeSpanParam*/0.5f, /*density*/14, true);
		ef->SetSca(m_smokeEffectScale);
		m_worldSmokeEffects.push_back(ef);
	}

	//for (int i = 0; i < numEffect; i++) {
	//	auto ef = NewGO<SmokeEffect>(0);
	//	ef->Init(Vector4::White, Vector4::White, false);
	//	ef->SetSca(m_smokeEffectScale);
	//	m_localSmokeEffects.push_back(ef);
	//}

	m_shieldBones[TO_INT(ShieldBone::Top)] = m_model->GetModel().GetSkelton()->GetBone(m_model->GetModel().GetSkelton()->FindBoneID(L"Top"));
	m_shieldBones[TO_INT(ShieldBone::Left)] = m_model->GetModel().GetSkelton()->GetBone(m_model->GetModel().GetSkelton()->FindBoneID(L"Left"));
	m_shieldBones[TO_INT(ShieldBone::Right)] = m_model->GetModel().GetSkelton()->GetBone(m_model->GetModel().GetSkelton()->FindBoneID(L"Right"));

	return true;
}


void Shield::PreUpdate()
{

}

void Shield::Update()
{
	m_model->SetPosition(m_position);
	m_model->SetRotation(m_rotation);
	m_model->SetScale(m_scale);
}

void Shield::PostUpdate()
{
	if (!m_isUsed) {
		m_model->SetActive(false);
		for (int i = 0; i < m_worldSmokeEffects.size(); i++) {
			m_worldSmokeEffects[i]->Stop();
			//m_localSmokeEffects[i]->Stop();
		}
	}
	else {
		m_model->SetActive(true);
		if (GameManager::GetInstance().m_menu != nullptr) {
			if (!GameManager::GetInstance().m_menu->IsGamePaused()) {
				m_activeTimer += gameTime()->GetDeltaTime();
			}
		}

		for (int i = 0; i < m_worldSmokeEffects.size(); i++) {
			m_worldSmokeEffects[i]->Play();
			m_worldSmokeEffects[i]->SetPos(m_shieldBones[i]->GetWorldMatrix().GetTransrate());
			m_worldSmokeEffects[i]->SetRot(m_shieldBones[i]->GetWorldMatrix().GetRotate());
			//m_localSmokeEffects[i]->Play();
			//m_localSmokeEffects[i]->SetPos(m_shieldBones[i]->GetWorldMatrix().GetTransrate());
			//m_localSmokeEffects[i]->SetRot(m_shieldBones[i]->GetWorldMatrix().GetRotate());
		}
	}
}


void Shield::Render()
{

}

void Shield::PostRender()
{

}
