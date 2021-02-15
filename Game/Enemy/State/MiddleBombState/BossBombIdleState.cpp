#include "stdafx.h"
#include "BossBombIdleState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Boss/Boss_MiddleBomb.h"
#include "GameManager.h"
#include "Game.h"
#include "Enemy/BossBGM.h"

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
	
	auto ik = e->GetIK(TO_INT(IEnemy::EnIK::enArm_R));
	m_oldIKPos = ik->GetEffectorBone()->GetWorldMatrix().GetTransrate();
	if (GameManager::GetInstance().m_bgm != nullptr) {

		GameManager::GetInstance().m_bgm->AddBgmCount(-1);
	}
}

IEnemyState* BossBombIdleState::Update(IEnemy* e)
{
	if(m_isAttacked)
		return e->GetState(TO_INT(IEnemy::EnState::enBattleState));

	m_timer += gameTime()->GetDeltaTime();

	e->SetExternalVelocity({ 0,-500,0 });
	
	auto p = GameManager::GetInstance().GetPlayer();
	auto d = p->GetPosition() - e->GetPosition();

	if (BossBombData::GetInstance().isStartBattle)
	{
		float t = m_timer / m_timeRotate;

		auto ik = e->GetIK(TO_INT(IEnemy::EnIK::enArm_R));

		if (t >= 1.f)
		{
			Quaternion rot;
			auto theta = atan2(d.x, d.z);
			rot.SetRotationY(theta);
			e->SetRotation(rot);

			ik->SetNextTarget(p->GetPosition());
		}
		else
		{
			Quaternion rot;
			auto theta = atan2(d.x, d.z);
			//theta = theta * (180.f / Math::PI);
			rot.SetRotationY(theta);
			rot.Slerp(t, m_firstRot, rot);
			e->SetRotation(rot);

			
			ik->SetNextTarget(Math::Lerp(t, m_oldIKPos, p->GetPosition()));
		}
	}

	float len = d.Length();
	if (len < 100.)
	{
		if (GameManager::GetInstance().m_bgm != nullptr) {

			GameManager::GetInstance().m_bgm->AddBgmCount(1);
		}
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
