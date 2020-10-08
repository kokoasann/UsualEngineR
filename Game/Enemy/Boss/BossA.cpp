#include "stdafx.h"
#include "BossA.h"

void BossA::Init() {

	//Model
	ModelInitData mid;
	mid.m_fxFilePath = "Assets/shader/AnimModel.fx";
	mid.m_vsEntryPointFunc = "VSMain";
	mid.m_psEntryPointFunc = "PSMain";
	mid.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	mid.m_tksFilePath = "Assets/modelData/unityChan.tks";
	mid.m_upAxis = enUpAxisY;

	m_model = NewGO<ModelRender>(0);
	m_model->Init(mid);
	//m_model->SetScale(g_vec3One * 0.05);
	m_model->SetScale(g_vec3One);

	//Init State
	SetState(m_stateList[enIdleState]);
}

void BossA::Terminate() {
	DeleteGO(m_model);
}

void BossA::Execute() {
	m_model->SetPosition(m_position);
}