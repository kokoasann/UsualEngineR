#include "stdafx.h"
#include "BossBombStunState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"

BossBombStunState::BossBombStunState()
{
}

BossBombStunState::~BossBombStunState()
{
}

void BossBombStunState::Enter(IEnemy* e)
{
	m_velocity = e->GetKnockBackImpulse();
	m_velDirXZ = m_velocity;
	m_velDirXZ.y = 0.f;
	m_velDirXZ.Normalize();
	m_velDirXZ *= -m_friction;
}

IEnemyState* BossBombStunState::Update(IEnemy* e)
{
	float dtime = gameTime()->GetDeltaTime();
	
	m_velocity.y -= 500.f * dtime;
	if (e->IsOnGround())
	{
		float velXZLen = sqrtf(powf(m_velocity.x, 2) + powf(m_velocity.z, 2));

		if (velXZLen > m_friction)
		{
			m_velocity += m_velDirXZ*dtime;
		}
		else
		{
			e->SetExternalVelocity(Vector3::Zero);
			return e->GetState(TO_INT(IEnemy::EnState::enBattleState));
		}
	}
	e->SetExternalVelocity(m_velocity);
	return this;
}

void BossBombStunState::Exit(IEnemy* e)
{
}

void BossBombStunState::OnAttacked(IEnemy* e)
{
}
