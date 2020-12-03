#include "stdafx.h"
#include "EnemyExplosionBombState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"

EnemyExplosionBombState::EnemyExplosionBombState()
{
}

EnemyExplosionBombState::~EnemyExplosionBombState()
{
}

void EnemyExplosionBombState::Enter(IEnemy* e)
{
}

IEnemyState* EnemyExplosionBombState::Update(IEnemy* e)
{

	return this;
}

void EnemyExplosionBombState::Exit(IEnemy* e)
{
}
