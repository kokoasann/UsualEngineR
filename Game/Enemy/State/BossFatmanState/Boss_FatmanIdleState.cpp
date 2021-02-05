#include "stdafx.h"
#include "Boss_FatmanIdleState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"

Boss_FatmanIdleState::Boss_FatmanIdleState()
{
}

Boss_FatmanIdleState::~Boss_FatmanIdleState()
{
}

void Boss_FatmanIdleState::Enter(IEnemy* e)
{
}

IEnemyState* Boss_FatmanIdleState::Update(IEnemy* e)
{
	return this;
}

void Boss_FatmanIdleState::Exit(IEnemy* e)
{
}

void Boss_FatmanIdleState::OnAttacked(IEnemy* e)
{
}
