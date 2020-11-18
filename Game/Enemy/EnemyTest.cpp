#include "stdafx.h"
#include "EnemyTest.h"

void EnemyTest::Init() {
	//Model
	ModelInitData mid;
	mid.m_tkmFilePath = "Assets/modelData/test/test.tkm";
	mid.m_vsEntryPointFunc = "VSMain";
	mid.m_psEntryPointFunc = "PSMain";
	//mid.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	//mid.m_tksFilePath = "Assets/modelData/unityChan.tks";
	mid.m_upAxis = enUpAxisY;

	m_model = NewGO<ModelRender>(0);
	m_model->Init(mid);
	m_model->SetScale(m_scale);
	m_animlist.resize(1);
	m_animlist[0] = std::make_unique<CAnimationClip>();
	m_animlist[0]->Load("Assets/anim/unityChan/run.tka");
	m_animlist[0]->BuildKeyFramesAndAnimationEvents();
	m_animlist[0]->SetLoopFlag(true);

	//m_model->InitAnimation(m_animlist, 1);
	//m_model->Play(0);

	//State
	SetState(m_stateList[static_cast<int>(IEnemy::EnState::enIdleState)]);

	//Physics
	InitCharacon(m_radius, m_height, m_position, true);

}


void EnemyTest::Terminate() {
	DeleteGO(m_model);
}

void EnemyTest::Execute() {
	m_model->SetPosition(m_position);
}

