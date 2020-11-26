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

	SetAnimation(TO_INT(IEnemy::EnAnimation::enIdle), "Assets/modelData/boss/sp/anim/sp_idle.tka", true);

	//State
	SetState(m_stateMap.at(TO_INT(IEnemy::EnState::enIdleState)));

	//Physics
	InitCharacon(m_radius, m_height, m_position, true);

}


void EnemyTest::Terminate() {
	DeleteGO(m_model);
}

void EnemyTest::Execute() {
	m_model->SetPosition(m_position);
}

