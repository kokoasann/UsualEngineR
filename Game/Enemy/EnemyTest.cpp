#include "stdafx.h"
#include "EnemyTest.h"
#include "IEnemyState.h"

void EnemyTest::Init() {

	ModelInitData mid;
	mid.m_fxFilePath = "Assets/shader/AnimModel.fx";
	mid.m_vsEntryPointFunc = "VSMain";
	mid.m_psEntryPointFunc = "PSMain";
	mid.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	mid.m_tksFilePath = "Assets/modelData/unityChan.tks";
	mid.m_upAxis = enUpAxisY;

	m_model = NewGO<ModelRender>(0);
	m_model->Init(mid);
	m_model->SetScale(g_vec3One * 0.05);
	m_animlist.resize(1);
	m_animlist[0] = std::make_unique<CAnimationClip>();
	m_animlist[0]->Load("Assets/anim/unityChan/run.tka");
	m_animlist[0]->BuildKeyFramesAndAnimationEvents();
	m_animlist[0]->SetLoopFlag(true);

	m_model->InitAnimation(m_animlist, 1);
	m_model->Play(0);

	SetState(m_stateList[enIdleState]);
}


void EnemyTest::Terminate() {
	DeleteGO(m_model);
}

void EnemyTest::Execute() {
	m_model->SetPosition(m_position);
}

