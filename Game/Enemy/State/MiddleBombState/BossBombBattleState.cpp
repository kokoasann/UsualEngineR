#include "stdafx.h"
#include "BossBombBattleState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Boss/Boss_MiddleBomb.h"
#include "GameManager.h"

BossBombBattleState::BossBombBattleState()
{
}

BossBombBattleState::~BossBombBattleState()
{
}

void BossBombBattleState::Enter(IEnemy* e)
{
	e->PlayAnimation(TO_INT(IEnemy::EnAnimation::enIdle));
	float perHP = e->GetCurrentHP()/e->GetMaxHP();
	if (perHP <= 2. / 3.)
	{
		m_isAngry = true;
		m_timeLimit = 1.;
	}

	m_timer = 0.f;

	e->SetAutoRotateFlag(false);

	m_firstRot = e->GetRotation();
}

IEnemyState* BossBombBattleState::Update(IEnemy* e)
{
	float dtime = gameTime()->GetDeltaTime();
	m_timer += dtime;

	float rand = GRandom().Rand();
	if (m_timer >= m_timeLimit && false)
	{
		
		if (rand < 0.4 || true)
		{
			return e->GetState(TO_INT(IEnemy::EnState::enAttackA));
		}
		else if (rand < 0.8)
		{
			return e->GetState(TO_INT(IEnemy::EnState::enAttackB));
		}
		else if (rand < 0.9)
		{
			return e->GetState(TO_INT(Boss_MiddleBomb::EnStateEX::FullFrontal));
		}
	}

	Vector3 move = Vector3::Zero;
	
	const auto& ppos = GameManager::GetInstance().GetPlayer()->GetPosition();
	const auto& epos = e->GetPosition();

	auto e2p = ppos - epos;
	float e2pLen = e2p.Length();

	if (e2pLen < m_distanceBash)
		return e->GetState(TO_INT(Boss_MiddleBomb::EnStateEX::Bash));

	auto e2pDir = e2p;
	e2pDir.Normalize();

	if (e2pLen < m_distance-20.f)
	{
		move += e2p * -m_speed * dtime;
	}
	else if (e2pLen > m_distance)
	{
		move += e2p * m_speed * dtime;
	}

	Quaternion rot;
	rot.SetRotationDegY(180.f * dtime);
	auto e2pRot = e2p;
	rot.Apply(e2pRot);
	e2pRot -= e2p;

	move += e2pRot;

	{
		float t = m_timer / m_timeRotate;
		if (t >= 1.f)
		{
			Quaternion rot;
			auto theta = atan2(e2p.x, e2p.z);
			rot.SetRotationY(theta);
			e->SetRotation(rot);
		}
		else
		{
			Quaternion rot;
			auto theta = atan2(e2p.x, e2p.z);
			//theta = theta * (180.f / Math::PI);
			rot.SetRotationY(theta);
			rot.Slerp(t, m_firstRot, rot);
			e->SetRotation(rot);
		}
	}

	e->SetVelocity(move);
	return this;
}

void BossBombBattleState::Exit(IEnemy* e)
{
}

void BossBombBattleState::OnAttacked(IEnemy* e)
{
}
