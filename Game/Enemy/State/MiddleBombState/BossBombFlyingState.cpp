#include "stdafx.h"
#include "BossBombFlyingState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"

BossBombFlyingState::BossBombFlyingState()
{
}

BossBombFlyingState::~BossBombFlyingState()
{
}

void BossBombFlyingState::Enter(IEnemy* e)
{
}

IEnemyState* BossBombFlyingState::Update(IEnemy* e)
{
	return this;
}

void BossBombFlyingState::Exit(IEnemy* e)
{
}

void BossBombFlyingState::OnAttacked(IEnemy* e)
{
}
