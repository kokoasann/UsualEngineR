#include "stdafx.h"
#include "EnemyLongDistanceAttackState.h"

void EnemyLongDistanceAttackState::Enter(IEnemy* e)
{
	m_timer = 0.f;
}

IEnemyState* EnemyLongDistanceAttackState::Update(IEnemy* e)
{
	float dtime = gameTime()->GetDeltaTime();
	m_timer += dtime;
	


	return nullptr;
}

void EnemyLongDistanceAttackState::Exit(IEnemy* e)
{
}
