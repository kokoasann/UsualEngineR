#include "stdafx.h"
#include "EnemyShortRangeComingState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"

EnemyShortRangeComingState::EnemyShortRangeComingState()
{
}

EnemyShortRangeComingState::~EnemyShortRangeComingState()
{
}

void EnemyShortRangeComingState::Enter(IEnemy* e)
{
}

IEnemyState* EnemyShortRangeComingState::Update(IEnemy* e)
{
	return this;
}

void EnemyShortRangeComingState::Exit(IEnemy* e)
{
}
