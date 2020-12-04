#include "stdafx.h"
#include "Boss_FatmanBeamState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"

Boss_FatmanBeamState::Boss_FatmanBeamState()
{
}

Boss_FatmanBeamState::~Boss_FatmanBeamState()
{
}

void Boss_FatmanBeamState::Enter(IEnemy* e)
{
}

IEnemyState* Boss_FatmanBeamState::Update(IEnemy* e)
{
	return this;
}

void Boss_FatmanBeamState::Exit(IEnemy* e)
{
}
