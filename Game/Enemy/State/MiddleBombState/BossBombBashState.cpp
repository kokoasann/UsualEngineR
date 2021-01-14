#include "stdafx.h"
#include "BossBombBashState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"

BossBombBashState::BossBombBashState()
{
}

BossBombBashState::~BossBombBashState()
{
}

void BossBombBashState::Enter(IEnemy* e)
{
}

IEnemyState* BossBombBashState::Update(IEnemy* e)
{
	return this;
}

void BossBombBashState::Exit(IEnemy* e)
{
}

void BossBombBashState::OnAttacked(IEnemy* e)
{
}
