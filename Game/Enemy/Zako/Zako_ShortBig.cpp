#include "stdafx.h"
#include "Zako_ShortBig.h"


Zako_ShortBig::Zako_ShortBig()
{

}

Zako_ShortBig::~Zako_ShortBig()
{

}

void Zako_ShortBig::Init()
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
void Zako_ShortBig::InitState()
{

}
void Zako_ShortBig::InitIK()
{

}

void Zako_ShortBig::Execute()
{
	m_model->SetPosition(m_position);
}

void Zako_ShortBig::Terminate()
{
	DeleteGO(m_model);
}
