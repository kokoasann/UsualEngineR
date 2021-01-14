#include "stdafx.h"
#include "BossBombJumpState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"

BossBombJumpState::BossBombJumpState()
{
}

BossBombJumpState::~BossBombJumpState()
{
}

void BossBombJumpState::Enter(IEnemy* e)
{
}

IEnemyState* BossBombJumpState::Update(IEnemy* e)
{
	return this;
}

void BossBombJumpState::Exit(IEnemy* e)
{
}

void BossBombJumpState::OnAttacked(IEnemy* e)
{
}
