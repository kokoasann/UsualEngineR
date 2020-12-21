#include "stdafx.h"
#include "Zako_ShortBig.h"
#include "Enemy/State/IEnemyState.h"
#include "Enemy/State/EnemyIdleState.h"
#include "Enemy/State/EnemyDeadState.h"

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
	m_stateMap.insert(std::make_pair(TO_INT(EnState::enIdleState), new EnemyIdleState()));
	//m_stateMap.insert(std::make_pair(TO_INT(EnState::enBattleState), new EnemyBattleState()));
	//m_stateMap.insert(std::make_pair(TO_INT(EnState::enAttackA), new EnemySlashState()));
	//m_stateMap.insert(std::make_pair(TO_INT(EnState::enStunState), new EnemyStunState()));
	//m_stateMap.insert(std::make_pair(TO_INT(EnState::enAttackB), new EnemySlashState()));
	//m_stateMap.insert(std::make_pair(TO_INT(EnState::enDeadState), new EnemySlashState()));
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
