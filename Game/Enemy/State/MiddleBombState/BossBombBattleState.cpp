#include "stdafx.h"
#include "BossBombBattleState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Boss/Boss_MiddleBomb.h"
#include "GameManager.h"

BossBombBattleState::BossBombBattleState()
{
}

BossBombBattleState::~BossBombBattleState()
{
}

void BossBombBattleState::Enter(IEnemy* e)
{
	float perHP = e->GetCurrentHP()/e->GetMaxHP();
	if (perHP <= 2. / 3.)
	{
		m_isAngry = true;
		m_timeLimit = 1.;
	}

	m_timer = 0.f;
}

IEnemyState* BossBombBattleState::Update(IEnemy* e)
{
	m_timer += gameTime()->GetDeltaTime();

	if (m_timer >= m_timeLimit)
	{
		float rand = GRandom().Rand();
		if (rand < 0.4 || true)
		{
			return e->GetState(TO_INT(IEnemy::EnState::enAttackA));
		}
		else if (rand < 0.8)
		{
			return e->GetState(TO_INT(IEnemy::EnState::enAttackB));
		}
		else if (rand < 0.9)
		{
			return e->GetState(TO_INT(Boss_MiddleBomb::EnStateEX::FullFrontal));
		}
	}
	


	return this;
}

void BossBombBattleState::Exit(IEnemy* e)
{
}

void BossBombBattleState::OnAttacked(IEnemy* e)
{
}
