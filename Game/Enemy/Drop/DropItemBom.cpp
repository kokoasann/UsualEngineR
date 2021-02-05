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
	if (m_effect != nullptr) {
		m_effect->Stop();
		DeleteGO(m_effect);
	}
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
	m_position.y += 10.f;

	m_effect = NewGO<SmokeEffect>(0);
	//effectの色を輝かせたいときは1.0fより上の数値に設定。
	//※densityは2.f未満だと表示されないようです
	m_effect->Init(Vector4(0.9f, 0.9f, 1.95f, 0.05f), Vector4(0.8f, 0.8f, 1.80f, 0.f), /*lifeSpanParam*/0.6f, /*density*/2.f, false);
	m_effect->SetSca(Vector3::One * 0.75f);
	//仮としてプレイヤーの位置
	//上にまき散らす。
	m_effect->SetPos(m_position);
	m_effect->SetSpeed(10.0f);
	m_effect->Play();

	return true;
}


void DropItemBom::PreUpdate()
{

}

void DropItemBom::Update()
{
	//deltaTimeためる変数
	m_deltaTimeSt += gameTime()->GetDeltaTime();
	//サインカーブの変化量をいれる 
	float sinValue = sin(m_PI * 2 * m_deltaTimeSt);
	//sinValue *= gameTime()->GetDeltaTime();
	const float rotateSpeed = 120.0f;
	m_angle += rotateSpeed * gameTime()->GetDeltaTime();


	Vector3 updownPos = m_position;
	updownPos.y += sinValue;
	m_model->SetPosition(updownPos);
	m_effect->SetPos(updownPos);

	Quaternion m_qrot;
	m_qrot.SetRotationDeg(Vector3::AxisY, m_angle);
	m_model->SetRotation(m_qrot);
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
