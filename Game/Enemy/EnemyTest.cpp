#include "stdafx.h"
#include "EnemyTest.h"
#include "State/IEnemyState.h"
#include "HealthBar.h"

void EnemyTest::Init() {

	ModelInitData mid;
	mid.m_tkmFilePath = "Assets/modelData/test/test_criss.tkm";
	mid.m_vsEntryPointFunc = "VSMain";
	mid.m_psEntryPointFunc = "PSMain";
	//mid.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	//mid.m_tksFilePath = "Assets/modelData/unityChan.tks";
	mid.m_upAxis = enUpAxisY;

	m_model = NewGO<ModelRender>(0);
	m_model->Init(mid);
	m_model->SetScale(g_vec3One);
	m_animlist.resize(1);
	m_animlist[0] = std::make_unique<CAnimationClip>();
	m_animlist[0]->Load("Assets/anim/unityChan/run.tka");
	m_animlist[0]->BuildKeyFramesAndAnimationEvents();
	m_animlist[0]->SetLoopFlag(true);

	//m_model->InitAnimation(m_animlist, 1);
	//m_model->Play(0);

	SetState(m_stateList[static_cast<int>(IEnemy::EnState::enIdleState)]);

	//HP Sprite
	m_healthBar = NewGO<HealthBar>(0);
}


void EnemyTest::Terminate() {
	DeleteGO(m_model);
	DeleteGO(m_healthBar);
}

void EnemyTest::Execute() {
	m_healthBar->SetParentPos(m_position);
	m_healthBar->SetHealthScale(GetCurrentHP() / GetMaxHP());
	m_model->SetPosition(m_position);
}

