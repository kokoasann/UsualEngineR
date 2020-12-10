#include "stdafx.h"
#include "Boss_FatmanShootingState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"

Boss_FatmanShootingState::Boss_FatmanShootingState()
{
}

Boss_FatmanShootingState::~Boss_FatmanShootingState()
{
}

void Boss_FatmanShootingState::Enter(IEnemy* e)
{
}

IEnemyState* Boss_FatmanShootingState::Update(IEnemy* e)
{
	return this;
}

void Boss_FatmanShootingState::Exit(IEnemy* e)
{
}
