#include "stdafx.h"
#include "Enemy/Zako/Zako_LongDistanceMachine.h"
#include "EnemyLongDistanceAttackState.h"
#include "EnemyLongDistanceTargetingState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/Equipment/Enemy_Bullet.h"

void EnemyLongDistanceAttackState::Init(IK* ik, float bulletSpeed, EnemyLongDistanceTargetingState* ldt)
{
	m_ik = ik;
	m_speed = bulletSpeed;
	m_ldt = ldt;
}

void EnemyLongDistanceAttackState::Enter(IEnemy* e)
{
	m_timer = 0.f;
	m_shotCount = 0;

	m_velocity = m_target - e->GetPosition();
	m_velocity.Normalize();

	e->PlayAnimation(TO_INT(Zako_LongDistanceMachine::EAnim::Fire));
}

IEnemyState* EnemyLongDistanceAttackState::Update(IEnemy* e)
{
	float dtime = gameTime()->GetDeltaTime();
	m_timer += dtime;
	m_ik->SetNextTarget(m_target);
	//ŽžŠÔ‚É‚È‚Á‚½‚ç’e‚ð”­ŽËB
	if (m_timer >= m_timeSpan)
	{
		Enemy_Bullet* eb = NewGO<Enemy_Bullet>(0,true);
		auto p = m_ik->GetEffectorBone()->GetWorldMatrix().GetTransrate();;
		
		//p.y += 10.;
		eb->Init(p, 1, m_velocity, m_speed, 5, 1);
		m_timer = 0.f;
		m_shotCount++;
		if (m_shotCount >= m_maxShot)
		{
			return m_ldt;
		}
	}
	
	return this;
}

void EnemyLongDistanceAttackState::Exit(IEnemy* e)
{
}
