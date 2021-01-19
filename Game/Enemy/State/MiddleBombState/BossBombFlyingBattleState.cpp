#include "stdafx.h"
#include "BossBombFlyingBattleState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"

BossBombFlyingBattleState::BossBombFlyingBattleState()
{
}

BossBombFlyingBattleState::~BossBombFlyingBattleState()
{
}

void BossBombFlyingBattleState::Enter(IEnemy* e)
{
}

IEnemyState* BossBombFlyingBattleState::Update(IEnemy* e)
{
	return this;
}

void BossBombFlyingBattleState::Exit(IEnemy* e)
{
}

void BossBombFlyingBattleState::OnAttacked(IEnemy* e)
{
}
