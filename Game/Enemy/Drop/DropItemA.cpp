#include "stdafx.h"
#include "DropItemA.h"
#include "../EnemyManager.h"
#include "../../Player/Player.h"
#include "GameManager.h"
#include "Game.h"

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
	mid.m_tkmFilePath = "Assets/modelData/test/test_criss.tkm";
	mid.m_upAxis = EUpAxis::enUpAxisY;
	mid.m_vsfxFilePath = "Assets/shader/NoAnimModel.fx";
	mid.m_vsEntryPointFunc = "VSMain";
	mid.m_psEntryPointFunc = "PSMain";

	m_model = NewGO<ModelRender>(0);
	m_model->Init(mid);
	m_model->SetScale(Vector3::One * m_scale);

	return true;
}


void DropItemA::PreUpdate()
{

}

void DropItemA::Update()
{
	m_model->SetPosition(m_position);
}

void DropItemA::PostUpdate()
{
	auto p = GameManager::GetInstance().m_player;
	auto dist = (p->GetPosition() - m_position).Length();

	if (dist < m_GRASP_RANGE) {
		//p->UnlockSkill(m_typeId);
		GameManager::GetInstance().m_gameScene->OnItemUnlocked();
		p->UnlockPreset(Player::EnAttackPreset::enMeleePreset);

		auto gameObj = reinterpret_cast<GameObject*>(this);
		DeleteGO(gameObj);
	}
}


void DropItemA::Render()
{

}

void DropItemA::PostRender()
{

}
