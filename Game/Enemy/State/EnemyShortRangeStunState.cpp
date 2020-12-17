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
	//e->PlayAnimation(TO_INT(IEnemy::EnAnimation::enIdle));

	if (m_ik == nullptr)
		m_ik = e->GetIK(TO_INT(IEnemy::EnIK::enChest));
	auto knockBack = e->GetKnockBackImpulse();
	auto knockBockVec = knockBack;
	knockBockVec.Normalize();
	float knockBackLen = knockBack.Length();
	//m_velocity = m_ik->GetEffectorBone()->GetWorldMatrix().GetTransrate();
	m_velocityEnd = knockBack/150.f;
	
	m_timeLimit = knockBackLen / 300.f;
	m_timer = 0.;
	m_t = 0.;

	m_knockBack = knockBack - knockBockVec*min(knockBackLen,150.f);
	m_up = m_knockBack.y * m_timeLimit;
	m_knockBack.y = 0;
	//m_knockBack /= 10.f;

	//e->SetKnockBackImpulse(Vector3::Zero);
}

IEnemyState* EnemyShortRangeStunState::Update(IEnemy* e)
{
	float dtime = gameTime()->GetDeltaTime();
	m_timer += dtime;

	if (m_timer >= m_timeLimit && e->IsOnGround())
	{
		e->SetExternalVelocity(Vector3::Zero);
		return e->GetState(TO_INT(IEnemy::EnState::enBattleState));
	}

	float t = m_timer / m_timeLimit;

	float s = (t - t * t) * 4.f;
	Vector3 p;
	p.Lerp(s, m_velocity, m_velocityEnd);
	m_ik->SetNextTarget(p + m_ik->GetEffectorBone()->GetWorldMatrix().GetTransrate());

	m_up -= m_grav * dtime;
	//m_knockBack * t;
	e->SetExternalVelocity({ m_knockBack.x,m_up,m_knockBack.z });
	
	return this;
}

void EnemyShortRangeStunState::Exit(IEnemy* e)
{
	e->SetExternalVelocity(Vector3::Zero);
}
