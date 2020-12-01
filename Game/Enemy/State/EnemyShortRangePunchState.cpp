#include "stdafx.h"
#include "EnemyShortRangePunchState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"

EnemyShortRangePunchState::EnemyShortRangePunchState()
{
}

EnemyShortRangePunchState::~EnemyShortRangePunchState()
{
}

void EnemyShortRangePunchState::Enter(IEnemy* e)
{
}

IEnemyState* EnemyShortRangePunchState::Update(IEnemy* e)
{
	return this;
}

void EnemyShortRangePunchState::Exit(IEnemy* e)
{
}
