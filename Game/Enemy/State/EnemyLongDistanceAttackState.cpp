#include "stdafx.h"
#include "EnemyLongDistanceAttackState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/Equipment/Enemy_Bullet.h"

void EnemyLongDistanceAttackState::Enter(IEnemy* e)
{
	m_timer = 0.f;
}

IEnemyState* EnemyLongDistanceAttackState::Update(IEnemy* e)
{
	float dtime = gameTime()->GetDeltaTime();
	m_timer += dtime;
	
	//ŽžŠÔ‚É‚È‚Á‚½‚ç’e‚ð”­ŽËB
	if (m_timer >= m_timeSpan)
	{
		Enemy_Bullet* eb = NewGO<Enemy_Bullet>(0);
		auto p = e->GetPosition();
		p.y += 10.;
		eb->Init(p, 1, { 0,1,0 }, 10, 10, 1);
		m_timer = 0.f;
	}

	return this;
}

void EnemyLongDistanceAttackState::Exit(IEnemy* e)
{
}
