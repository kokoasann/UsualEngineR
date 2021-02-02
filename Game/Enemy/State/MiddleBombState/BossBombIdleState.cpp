#include "stdafx.h"
#include "BossBombIdleState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Boss/Boss_MiddleBomb.h"
#include "GameManager.h"

BossBombIdleState::BossBombIdleState()
{
}

BossBombIdleState::~BossBombIdleState()
{
}

void BossBombIdleState::Enter(IEnemy* e)
{
	e->PlayAnimation(TO_INT(IEnemy::EnAnimation::enIdle));

	e->SetVelocity(Vector3::Zero);

	m_timer = 0.f;
	m_firstRot = e->GetRotation();
	m_isAttacked = false;

	e->SetAutoRotateFlag(false);
}

IEnemyState* BossBombIdleState::Update(IEnemy* e)
{
	if(m_isAttacked)
		return e->GetState(TO_INT(IEnemy::EnState::enBattleState));

	e->SetExternalVelocity({ 0,-500,0 });
	
	auto p = GameManager::GetInstance().GetPlayer();
	auto d = p->GetPosition() - e->GetPosition();

	if (BossBombData::GetInstance().isStartBattle)
	{
		float t = m_timer / m_timeRotate;
		if (t >= 1.f)
		{
			Quaternion rot;
			auto theta = atan2(d.x, d.z);
			rot.SetRotationY(theta);
			e->SetRotation(rot);
		}
		else
		{
			Quaternion rot;
			auto theta = atan2(d.x, d.z);
			//theta = theta * (180.f / Math::PI);
			rot.SetRotationY(theta);
			rot.Slerp(t, m_firstRot, rot);
			e->SetRotation(rot);
		}
	}

	float len = d.Length();
	if (len < 100.)
	{
		return e->GetState(TO_INT(IEnemy::EnState::enBattleState));
	}
	return this;
}

void BossBombIdleState::Exit(IEnemy* e)
{
}

void BossBombIdleState::OnAttacked(IEnemy* e)
{
	m_isAttacked = true;
}
