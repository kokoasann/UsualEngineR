#include "stdafx.h"
#include "Zako_ShortRangeMonster.h"


Zako_ShortRangeMonster::Zako_ShortRangeMonster()
{

}

Zako_ShortRangeMonster::~Zako_ShortRangeMonster()
{

}

void Zako_ShortRangeMonster::Init()
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
void Zako_ShortRangeMonster::InitState()
{

}

void Zako_ShortRangeMonster::Execute()
{
	m_model->SetPosition(m_position);
}

void Zako_ShortRangeMonster::Terminate()
{
	DeleteGO(m_model);
}
