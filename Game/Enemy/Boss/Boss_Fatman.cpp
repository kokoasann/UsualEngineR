#include "stdafx.h"
#include "Boss_Fatman.h"


Boss_Fatman::Boss_Fatman()
{

}

Boss_Fatman::~Boss_Fatman()
{

}

void Boss_Fatman::Init()
{
	//Model
	ModelInitData mid;
	mid.m_tkmFilePath = "Assets/modelData/test/test.tkm";
	mid.m_vsEntryPointFunc = "VSMain";
	mid.m_psEntryPointFunc = "PSMain";
	mid.m_upAxis = enUpAxisY;

	m_model = NewGO<ModelRender>(0);
	m_model->Init(mid);
	m_model->SetScale(Vector3::One * m_scale);

	//State
	SetState(m_stateMap[static_cast<int>(IEnemy::EnState::enIdleState)]);

	//Physics
	InitCharacon(m_radius, m_height, m_position, true);
}
void Boss_Fatman::InitState()
{

}

void Boss_Fatman::Execute()
{
	m_model->SetPosition(m_position);
}

void Boss_Fatman::Terminate()
{
	DeleteGO(m_model);
}
