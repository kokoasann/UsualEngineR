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
	{
		auto p = std::make_pair(TO_INT(IEnemy::EnState::enDeadState), new EnemyDeadState());
		m_stateMap.insert(p);
	}
	{
		auto p = std::make_pair(TO_INT(IEnemy::EnState::enIdleState), new EnemyIdleState());
		m_stateMap.insert(p);
	}
	{
		auto p = std::make_pair(TO_INT(EnStateEX::enComing), new EnemyShortRangeComingState());
		m_stateMap.insert(p);
	}
}

void Zako_ShortRangeMonster::Execute()
{
	m_model->SetPosition(m_position);
	//m_model->SetRotation(m_rot);

	//体力がなくなったら死亡ステートへ遷移
	if (m_ability.hp <= 0) {
		SetState(m_stateMap[TO_INT(IEnemy::EnState::enDeadState)]);
	}
}

void Zako_ShortRangeMonster::Terminate()
{
	DeleteGO(m_model);
}
