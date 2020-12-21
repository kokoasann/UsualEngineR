#include "stdafx.h"
#include "EnemyShortBigIdle.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"

EnemyShortBigIdle::EnemyShortBigIdle()
{
}

EnemyShortBigIdle::~EnemyShortBigIdle()
{
}

void EnemyShortBigIdle::Enter(IEnemy* e)
{
}

IEnemyState* EnemyShortBigIdle::Update(IEnemy* e)
{
	return this;
}

void EnemyShortBigIdle::Exit(IEnemy* e)
{
}

void EnemyShortBigIdle::OnAttacked(IEnemy* e)
{
}
