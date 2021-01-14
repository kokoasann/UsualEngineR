#include "stdafx.h"
#include "BossBombFullFrontalState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"

BossBombFullFrontalState::BossBombFullFrontalState()
{
}

BossBombFullFrontalState::~BossBombFullFrontalState()
{
}

void BossBombFullFrontalState::Enter(IEnemy* e)
{
}

IEnemyState* BossBombFullFrontalState::Update(IEnemy* e)
{
	return this;
}

void BossBombFullFrontalState::Exit(IEnemy* e)
{
}

void BossBombFullFrontalState::OnAttacked(IEnemy* e)
{
}
