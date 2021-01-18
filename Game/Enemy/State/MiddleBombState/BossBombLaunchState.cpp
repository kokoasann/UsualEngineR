#include "stdafx.h"
#include "BossBombLaunchState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"

BossBombLaunchState::BossBombLaunchState()
{
}

BossBombLaunchState::~BossBombLaunchState()
{
}

void BossBombLaunchState::Enter(IEnemy* e)
{
}

IEnemyState* BossBombLaunchState::Update(IEnemy* e)
{
	return this;
}

void BossBombLaunchState::Exit(IEnemy* e)
{
}

void BossBombLaunchState::OnAttacked(IEnemy* e)
{
}
