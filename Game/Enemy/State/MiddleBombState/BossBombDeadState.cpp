#include "stdafx.h"
#include "BossBombDeadState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"

BossBombDeadState::BossBombDeadState()
{
}

BossBombDeadState::~BossBombDeadState()
{
}

void BossBombDeadState::Enter(IEnemy* e)
{
}

IEnemyState* BossBombDeadState::Update(IEnemy* e)
{
	return this;
}

void BossBombDeadState::Exit(IEnemy* e)
{
}

void BossBombDeadState::OnAttacked(IEnemy* e)
{
}
