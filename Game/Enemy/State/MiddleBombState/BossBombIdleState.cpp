#include "stdafx.h"
#include "BossBombIdleState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"

BossBombIdleState::BossBombIdleState()
{
}

BossBombIdleState::~BossBombIdleState()
{
}

void BossBombIdleState::Enter(IEnemy* e)
{
}

IEnemyState* BossBombIdleState::Update(IEnemy* e)
{
	return this;
}

void BossBombIdleState::Exit(IEnemy* e)
{
}

void BossBombIdleState::OnAttacked(IEnemy* e)
{
}
