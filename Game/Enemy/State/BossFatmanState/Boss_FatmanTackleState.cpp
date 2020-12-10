#include "stdafx.h"
#include "Boss_FatmanTackleState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"

Boss_FatmanTackleState::Boss_FatmanTackleState()
{
}

Boss_FatmanTackleState::~Boss_FatmanTackleState()
{
}

void Boss_FatmanTackleState::Enter(IEnemy* e)
{
}

IEnemyState* Boss_FatmanTackleState::Update(IEnemy* e)
{
	return this;
}

void Boss_FatmanTackleState::Exit(IEnemy* e)
{
}
