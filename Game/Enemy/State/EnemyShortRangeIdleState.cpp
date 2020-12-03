#include "stdafx.h"
#include "EnemyShortRangeIdleState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"

EnemyShortRangeIdleState::EnemyShortRangeIdleState()
{
}

EnemyShortRangeIdleState::~EnemyShortRangeIdleState()
{
}

void EnemyShortRangeIdleState::Enter(IEnemy* e)
{
}

IEnemyState* EnemyShortRangeIdleState::Update(IEnemy* e)
{
	return this;
}

void EnemyShortRangeIdleState::Exit(IEnemy* e)
{
}
