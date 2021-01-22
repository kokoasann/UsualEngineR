#include "stdafx.h"
#include "BossBombGuardState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Boss/Boss_MiddleBomb.h"
#include "GameManager.h"

BossBombGuardState::BossBombGuardState()
{
}

BossBombGuardState::~BossBombGuardState()
{
}

void BossBombGuardState::Enter(IEnemy* e)
{
	e->PlayAnimation(TO_INT(Boss_MiddleBomb::EnAnimEX::Guard));
	m_timer = 0.;
}

IEnemyState* BossBombGuardState::Update(IEnemy* e)
{
	m_timer += gameTime()->GetDeltaTime();
	if (m_timer > m_timeLimit)
	{
		return e->GetState(TO_INT(IEnemy::EnState::enBattleState));
	}


	return this;
}

void BossBombGuardState::Exit(IEnemy* e)
{
}

void BossBombGuardState::OnAttacked(IEnemy* e)
{
}
