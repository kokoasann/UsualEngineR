#include "stdafx.h"
#include "EnemyShortRangeStunState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"

EnemyShortRangeStunState::EnemyShortRangeStunState()
{
}

EnemyShortRangeStunState::~EnemyShortRangeStunState()
{
}

void EnemyShortRangeStunState::Enter(IEnemy* e)
{
	e->PlayAnimation(TO_INT(IEnemy::EnAnimation::enIdle));

	if (m_ik == nullptr)
		m_ik = e->GetIK(TO_INT(IEnemy::EnIK::enChest));
	m_velocity = m_ik->GetEffectorBone()->GetWorldMatrix().GetTransrate();
	m_velocityEnd = m_velocity + e->GetKnockBackImpulse()/100;
	e->SetKnockBackImpulse(Vector3::Zero);
	m_timeLimit = m_velocityEnd.Length() / 1000.f;
	m_timer = 0.;
	m_t = 0.;
}

IEnemyState* EnemyShortRangeStunState::Update(IEnemy* e)
{
	m_timer += gameTime()->GetDeltaTime();

	if (m_timer >= m_timeLimit)
		return e->GetState(TO_INT(IEnemy::EnState::enBattleState));

	float t = m_timer / m_timeLimit;

	float s = (t - t * t) * 4.f;
	Vector3 p;
	p.Lerp(s, m_velocity, m_velocityEnd);
	m_ik->SetNextTarget(p);

	return this;
}

void EnemyShortRangeStunState::Exit(IEnemy* e)
{
}
