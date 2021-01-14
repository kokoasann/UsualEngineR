#include "stdafx.h"
#include "Boss_MiddleBomb.h"


Boss_MiddleBomb::Boss_MiddleBomb()
{

}

Boss_MiddleBomb::~Boss_MiddleBomb()
{

}

void Boss_MiddleBomb::Init()
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
void Boss_MiddleBomb::InitState()
{

}
void Boss_MiddleBomb::InitIK()
{

}

void Boss_MiddleBomb::Execute()
{
	m_model->SetPosition(m_position);
}

void Boss_MiddleBomb::Terminate()
{
	DeleteGO(m_model);
}
