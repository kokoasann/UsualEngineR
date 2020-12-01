#include "stdafx.h"
#include "EnemyShortRangeDanceState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"

EnemyShortRangeDanceState::EnemyShortRangeDanceState()
{
}

EnemyShortRangeDanceState::~EnemyShortRangeDanceState()
{
}

void EnemyShortRangeDanceState::Enter(IEnemy* e)
{
}

IEnemyState* EnemyShortRangeDanceState::Update(IEnemy* e)
{
	return this;
}

void EnemyShortRangeDanceState::Exit(IEnemy* e)
{
}
