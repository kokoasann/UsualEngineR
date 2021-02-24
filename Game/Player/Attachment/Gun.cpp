#include "stdafx.h"
#include "Gun.h"
#include "Effect/MuzzleFlash.h"
#include "GameManager.h"
#include "GameSceneMenu.h"

Gun::Gun()
{

}

Gun::~Gun()
{

}



void Gun::Release()
{
	DeleteGO(m_model);
	for (int i = 0; i < m_muzzleFlashes.size(); i++) {
		DeleteGO(m_muzzleFlashes.at(i));
	}
}

void Gun::OnDestroy()
{
	Release();
}


void Gun::Awake()
{

}

bool Gun::Start()
{
	ModelInitData mid;
	m_model = NewGO<ModelRender>(0);
	mid.m_tkmFilePath = "Assets/modelData/m/Gun/Gun.tkm";
	mid.m_tksFilePath = "Assets/modelData/m/Gun/Gun.tks";
	//mid.m_upAxis = enUpAxisZ;
	m_model->Init(mid);

	m_gunBones[TO_INT(GunBone::Left)] = m_model->GetModel().GetSkelton()->GetBone(m_model->GetModel().GetSkelton()->FindBoneID(L"Left"));
	m_gunBones[TO_INT(GunBone::Right)] = m_model->GetModel().GetSkelton()->GetBone(m_model->GetModel().GetSkelton()->FindBoneID(L"Right"));

	//Effect
	MuzzleFlashEffectInitData mfid;
	for (int i = 0; i < TO_INT(EnMuzzles::NumMuzzles); i++) {
		auto muzzleFlash = NewGO<MuzzleFlash>(0);
		muzzleFlash->Init(mfid);
		muzzleFlash->SetSca(Vector3::One *0.1);
		m_muzzleFlashes.push_back(muzzleFlash);
	}

	return true;
}


void Gun::PreUpdate()
{

}

void Gun::Update()
{
	m_model->SetPosition(m_position);
	m_model->SetRotation(m_rotation);
	m_model->SetScale(m_scale);
}

void Gun::PostUpdate()
{

	for (int i = 0; i < m_muzzleFlashes.size(); i++) {
		m_muzzleFlashes[i]->SetPos(m_gunBones[i]->GetWorldMatrix().GetTransrate());
		m_muzzleFlashes[i]->SetRot(m_gunBones[i]->GetWorldMatrix().GetRotate());
	}

	if (!m_isUsed) {
		m_model->SetActive(false);
	}
	else {
		m_model->SetActive(true);
		if (GameManager::GetInstance().m_menu != nullptr) {
			if (!GameManager::GetInstance().m_menu->IsGamePaused()) {
				m_activeTimer += gameTime()->GetDeltaTime();
			}
		}
	}
}


void Gun::Render()
{

}

void Gun::PostRender()
{

}

void Gun::PlayFireEffects() {
	for (int i = 0; i < m_muzzleFlashes.size(); i++) {
		m_muzzleFlashes[i]->Play();
	}
}
