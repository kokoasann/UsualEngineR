#include "stdafx.h"
#include "DropItemA.h"
#include "../EnemyManager.h"
#include "../../Player/Player.h"
#include "GameManager.h"
#include "Game.h"
#include "Effect/SmokeEffect.h"

DropItemA::DropItemA()
{

}

DropItemA::~DropItemA()
{

}



void DropItemA::Release()
{
	DeleteGO(m_model);
}

void DropItemA::OnDestroy()
{
	Release();
}


void DropItemA::Awake()
{

}

bool DropItemA::Start()
{
	//Model
	ModelInitData mid;
	mid.m_tkmFilePath = "Assets/modelData/m/BackPack/BackPack.tkm";
	mid.m_upAxis = EUpAxis::enUpAxisY;
	mid.m_vsfxFilePath = "Assets/shader/NoAnimModel.fx";
	mid.m_vsEntryPointFunc = "VSMain";
	mid.m_psEntryPointFunc = "PSMain";

	m_model = NewGO<ModelRender>(0);
	m_model->Init(mid);
	m_model->SetScale(Vector3::One * m_scale);
	m_position.y += 10.f;

	m_effect = NewGO<SmokeEffect>(0);
	m_effect->Init(Vector4(2.98f, 2.92f, 1.5f, 0.2f), Vector4(0.92f, 0.93f, 0.2f, 0.f), 0.1f, 2.f, false);
	m_effect->SetSca(Vector3::One * 0.45f);
	m_effect->SetPos(m_position);
	m_effect->Play();

	return true;
}


void DropItemA::PreUpdate()
{

}

void DropItemA::Update()
{
	float rotSpeed = 50.f;
	Quaternion rot;
	rot.SetRotationDeg(Vector3::AxisY, rotSpeed * gameTime()->GetDeltaTime());
	m_rotation.Multiply(rot);
	m_model->SetRotation(m_rotation);

	m_position.y += sin(m_count) * 8.f * gameTime()->GetDeltaTime();
	m_count += 10.0f * gameTime()->GetDeltaTime();
	m_model->SetPosition(m_position);

	m_effect->SetPos(m_position);
}

void DropItemA::PostUpdate()
{
	auto p = GameManager::GetInstance().m_player;

	if (p == nullptr) {
		auto go = reinterpret_cast<GameObject*>(this);
		DeleteGO(go);

		if (m_effect != nullptr) {
			m_effect->Stop();
			DeleteGO(m_effect);
		}
		return;
	}

	auto dist = (p->GetPosition() - m_position).Length();

	if (dist < m_GRASP_RANGE) {
		//p->UnlockSkill(m_typeId);
		GameManager::GetInstance().m_gameScene->OnItemUnlocked();
		p->UnlockPreset(Player::EnAttackPreset::enMeleePreset);

		auto gameObj = reinterpret_cast<GameObject*>(this);
		DeleteGO(gameObj);

		if (m_effect != nullptr) {
			m_effect->Stop();
			DeleteGO(m_effect);
		}
	}
}


void DropItemA::Render()
{

}

void DropItemA::PostRender()
{

}
