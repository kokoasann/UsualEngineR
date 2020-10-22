#include "stdafx.h"
#include "BossA.h"

void BossA::Init() {

	//Model
	ModelInitData mid;
	mid.m_tkmFilePath = "Assets/modelData/test/test_criss.tkm";
	mid.m_upAxis = EUpAxis::enUpAxisY;
	mid.m_vsfxFilePath = "Assets/shader/NoAnimModel.fx";
	mid.m_vsEntryPointFunc = "VSMain";
	mid.m_psEntryPointFunc = "PSMain";

	m_model = NewGO<ModelRender>(0);
	m_model->Init(mid);
	//m_model->SetScale(g_vec3One * 0.05);
	m_model->SetScale(g_vec3One * 10);

	//Init State
	SetState(m_stateList[static_cast<int>(IEnemy::EnState::enIdleState)]);
	m_isDrawHpBarAboveMyself = false;

	//Physics
	InitCharacon(m_radius,m_height, m_position, true);
}

void BossA::Terminate() {
	DeleteGO(m_model);
}

void BossA::Execute() {
	m_model->SetPosition(m_position);
}