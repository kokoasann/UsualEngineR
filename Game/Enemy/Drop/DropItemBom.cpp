#include "stdafx.h"
#include "DropItemBom.h"
#include "../EnemyManager.h"
#include "../../Player/Player.h"
#include "GameManager.h"
#include "Game.h"

DropItemBom::DropItemBom()
{

}

DropItemBom::~DropItemBom()
{

}



void DropItemBom::Release()
{
	DeleteGO(m_model);
}

void DropItemBom::OnDestroy()
{
	Release();
}


void DropItemBom::Awake()
{

}

bool DropItemBom::Start()
{
	//Model
	ModelInitData mid;
	mid.m_tkmFilePath = "Assets/modelData/m/Shield/Shield.tkm";
	mid.m_upAxis = EUpAxis::enUpAxisY;
	mid.m_vsfxFilePath = "Assets/shader/NoAnimModel.fx";
	mid.m_vsEntryPointFunc = "VSMain";
	mid.m_psEntryPointFunc = "PSMain";

	m_model = NewGO<ModelRender>(0);
	m_model->Init(mid);
	m_model->SetScale(Vector3::One * m_scale);

	return true;
}


void DropItemBom::PreUpdate()
{

}

void DropItemBom::Update()
{
	m_model->SetPosition(m_position);
}

void DropItemBom::PostUpdate()
{
	auto p = GameManager::GetInstance().m_player;

	if (p == nullptr) {
		auto go = reinterpret_cast<GameObject*>(this);
		DeleteGO(go);
		return;
	}

	auto dist = (p->GetPosition() - m_position).Length();

	if (dist < m_GRASP_RANGE) {
		//p->UnlockSkill(m_typeId);
		GameManager::GetInstance().m_gameScene->OnItemUnlocked();
		p->UnlockPreset(Player::EnAttackPreset::enMeleePreset);

		auto gameObj = reinterpret_cast<GameObject*>(this);
		DeleteGO(gameObj);
	}
}


void DropItemBom::Render()
{

}

void DropItemBom::PostRender()
{

}
