#include "stdafx.h"
#include "Boss_FatmanDeadState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"

Boss_FatmanDeadState::Boss_FatmanDeadState()
{
}

Boss_FatmanDeadState::~Boss_FatmanDeadState()
{
}

void Boss_FatmanDeadState::Enter(IEnemy* e)
{
}

IEnemyState* Boss_FatmanDeadState::Update(IEnemy* e)
{
	return this;
}

void Boss_FatmanDeadState::Exit(IEnemy* e)
{
}

void Boss_FatmanDeadState::OnAttacked(IEnemy* e)
{
}
