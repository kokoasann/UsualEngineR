#include "stdafx.h"
#include "BossBombCoroCoroState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"

BossBombCoroCoroState::BossBombCoroCoroState()
{
}

BossBombCoroCoroState::~BossBombCoroCoroState()
{
}

void BossBombCoroCoroState::Enter(IEnemy* e)
{
}

IEnemyState* BossBombCoroCoroState::Update(IEnemy* e)
{
	return this;
}

void BossBombCoroCoroState::Exit(IEnemy* e)
{
}

void BossBombCoroCoroState::OnAttacked(IEnemy* e)
{
}
