#include "stdafx.h"
#include "Boss_FatmanMainState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"

Boss_FatmanMainState::Boss_FatmanMainState()
{
}

Boss_FatmanMainState::~Boss_FatmanMainState()
{
}

void Boss_FatmanMainState::Enter(IEnemy* e)
{
}

IEnemyState* Boss_FatmanMainState::Update(IEnemy* e)
{
	return this;
}

void Boss_FatmanMainState::Exit(IEnemy* e)
{
}
