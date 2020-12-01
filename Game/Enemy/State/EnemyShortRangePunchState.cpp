#include "stdafx.h"
#include "EnemyShortRangePunchState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "GameManager.h"

EnemyShortRangePunchState::EnemyShortRangePunchState()
{
}

EnemyShortRangePunchState::~EnemyShortRangePunchState()
{
}

void EnemyShortRangePunchState::Enter(IEnemy* e)
{
}

IEnemyState* EnemyShortRangePunchState::Update(IEnemy* e)
{
	Attack(e);
	return this;
}

void EnemyShortRangePunchState::Exit(IEnemy* e)
{
}

void EnemyShortRangePunchState::Attack(IEnemy* e)
{
	//30ƒtƒŒ[ƒ€‚¨‚«‚É‰£‚éB
	if (m_attackIntervalCount >= 30)
	{
		auto& p = GameManager::GetInstance().m_player;
		//p->ApplyDamage();
	}
}
