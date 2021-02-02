#include "stdafx.h"
#include "BossBombRollingState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"

BossBombRollingState::BossBombRollingState()
{
}

BossBombRollingState::~BossBombRollingState()
{
}

void BossBombRollingState::Enter(IEnemy* e)
{
}

IEnemyState* BossBombRollingState::Update(IEnemy* e)
{
	return this;
}

void BossBombRollingState::Exit(IEnemy* e)
{
}

void BossBombRollingState::OnAttacked(IEnemy* e)
{
}
