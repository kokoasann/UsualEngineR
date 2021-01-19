#include "stdafx.h"
#include "Boss_FatmanChargeBeamAndShootingState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"

Boss_FatmanChargeBeamAndShootingState::Boss_FatmanChargeBeamAndShootingState()
{
}

Boss_FatmanChargeBeamAndShootingState::~Boss_FatmanChargeBeamAndShootingState()
{
}

void Boss_FatmanChargeBeamAndShootingState::Enter(IEnemy* e)
{
}

IEnemyState* Boss_FatmanChargeBeamAndShootingState::Update(IEnemy* e)
{
	return this;
}

void Boss_FatmanChargeBeamAndShootingState::Exit(IEnemy* e)
{
}

void Boss_FatmanChargeBeamAndShootingState::OnAttacked(IEnemy* e)
{
}
