#include "stdafx.h"
#include "BossBombGuardState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"

BossBombGuardState::BossBombGuardState()
{
}

BossBombGuardState::~BossBombGuardState()
{
}

void BossBombGuardState::Enter(IEnemy* e)
{
}

IEnemyState* BossBombGuardState::Update(IEnemy* e)
{
	return this;
}

void BossBombGuardState::Exit(IEnemy* e)
{
}

void BossBombGuardState::OnAttacked(IEnemy* e)
{
}
