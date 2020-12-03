#include "stdafx.h"
#include "Zako_ExplosionMonster.h"


Zako_ExplosionMonster::Zako_ExplosionMonster()
{

}

Zako_ExplosionMonster::~Zako_ExplosionMonster()
{

}

void Zako_ExplosionMonster::Init()
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

	InitAnimation();

	//State
	SetState(m_stateMap[TO_INT(IEnemy::EnState::enIdleState)]);

	//Physics
	InitCharacon(m_radius, m_height, m_position, true);
}

void Zako_ExplosionMonster::InitAnimation()
{
}

void Zako_ExplosionMonster::InitState()
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
		auto p = std::make_pair(TO_INT(IEnemy::EnState::enBattleState), new EnemyExplosionKamikazeState());
		m_stateMap.insert(p);
	}
	{
		auto p = std::make_pair(TO_INT(IEnemy::EnState::enAttackA), new EnemyExplosionBombState());
		m_stateMap.insert(p);
	}
}

void Zako_ExplosionMonster::Execute()
{
	m_model->SetPosition(m_position);
	m_model->SetRotation(m_rotation);

	//体力がなくなったら死亡ステートへ遷移
	if (m_ability.hp <= 0) {
		SetState(m_stateMap[TO_INT(IEnemy::EnState::enDeadState)]);
	}
}

void Zako_ExplosionMonster::Terminate()
{
	DeleteGO(m_model);
}