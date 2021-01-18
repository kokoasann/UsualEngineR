#include "stdafx.h"
#include "BossBombBattleState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"

BossBombBattleState::BossBombBattleState()
{
}

BossBombBattleState::~BossBombBattleState()
{
}

void BossBombBattleState::Enter(IEnemy* e)
{
}

IEnemyState* BossBombBattleState::Update(IEnemy* e)
{
	return this;
}

void BossBombBattleState::Exit(IEnemy* e)
{
}

void BossBombBattleState::OnAttacked(IEnemy* e)
{
}
