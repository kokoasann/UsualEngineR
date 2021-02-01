#include "stdafx.h"
#include "BossBombCoroCoroState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Boss/Boss_MiddleBomb.h"
#include "GameManager.h"

#include "Enemy/Equipment/Enemy_CorocoroBomb.h"

BossBombCoroCoroState::BossBombCoroCoroState()
{
}

BossBombCoroCoroState::~BossBombCoroCoroState()
{
}

void BossBombCoroCoroState::Enter(IEnemy* e)
{

	e->PlayAnimation(TO_INT(IEnemy::EnAnimation::enAttackA));

	auto ik = e->GetIK(TO_INT(IEnemy::EnIK::enArm_L));
	auto bone = ik->GetEffectorBone();
	if (!m_cannonBone)
		m_cannonBone = e->GetModel()->GetModel().GetSkelton()->GetBone(bone->GetParentBoneNo());
	e->SetAutoRotateFlag(false);

	m_firstRot = e->GetRotation();
	m_timer = 0.f;
	m_isLaunch = false;
	m_launceNum = 0;

	m_timeLaunch = 0.f;
	m_isLaunchSleep = false;
}

IEnemyState* BossBombCoroCoroState::Update(IEnemy* e)
{
	m_timer += gameTime()->GetDeltaTime();

	

	float t = m_timer / m_timeLimit;
	t = min(t, 1.f);

	auto ik = e->GetIK(TO_INT(IEnemy::EnIK::enArm_L));
	auto bone = ik->GetEffectorBone();

	auto& ppos = GameManager::GetInstance().GetPlayer()->GetPosition();
	auto& epos = e->GetPosition();

	auto e2p = ppos - epos;
	e2p.y = 0.;
	float e2pLen = e2p.Length();
	e2p.Normalize();

	Vector3 ikpos = epos;
	if (BossBombData::GetInstance().feeling == BossBombData::EnFeel::Normale)
	{
		auto ikpos = ppos;
		ikpos.y = epos.y;
		ikpos -= e2p * 5.f;
		ik->SetNextTarget(ikpos);
	}
	else
	{
		float t = min((m_timer - (m_timeLaunch - m_timeLaunchSpan)) / m_timeLaunchSpan,1.f);
		
		Quaternion rot;
		Vector3 e2pDir = e2p;
		switch (m_launceNum)
		{
		case 0:
			rot.SetRotationDegY(0.f);
			rot.Apply(e2pDir);
			ikpos += e2pDir * max(e2pLen - 5.f, 5.f);
			
			break;
		case 1:
			rot.SetRotationDegY(5.f);
			rot.Apply(e2pDir);
			ikpos += e2pDir * max(e2pLen - 5.f, 5.f);
			ikpos = Math::Lerp(t, m_oldIKpos, ikpos);
			break;
		case 2:
			rot.SetRotationDegY(15.f);
			rot.Apply(e2pDir);
			ikpos += e2pDir * max(e2pLen - 5.f, 5.f);
			ikpos = Math::Lerp(t, m_oldIKpos, ikpos);
			break;
		}
		
		ik->SetNextTarget(ikpos);
	}
	
	//Quaternion rot(Vector3::Front, e2p);
	{
		Quaternion rot;
		auto theta = atan2(e2p.x, e2p.z);
		//theta = theta * (180.f / Math::PI);
		rot.SetRotationY(theta);
		rot.Slerp(t, m_firstRot, rot);
		e->SetRotation(rot);
	}

	//
	if (m_isLaunchSleep)
	{
		if (m_timer >= m_timeLaunch)
			m_isLaunchSleep = false;
		else
			return this;
	}


	if (e->GetModel()->IsAnimPlaying())
	{
		//if(BossBombData::GetInstance().feeling == BossBombData::EnFeel::Normale)
			
		return this;
	}

	if ((m_timer-m_timeAnimEnd) - m_timeEnd >= 0.f && m_isLaunch)
		return e->GetState(TO_INT(IEnemy::EnState::enBattleState));
	

	if (!m_isLaunch)
	{
		const auto& mat = bone->GetWorldMatrix();
		const auto& pmat = m_cannonBone->GetWorldMatrix();

		Vector3 v = Vector3::Up;
		pmat.GetRotate().Apply(v);
		//v.y += 0.3;
		auto bonePos = mat.GetTransrate();

		Enemy_CorocoroBomb* bomb = NewGO<Enemy_CorocoroBomb>(0, true);

		bomb->Init(bonePos, v * 100, 1, 20, 1, 25,1.5f);

		m_launceNum++;
		if (BossBombData::GetInstance().feeling == BossBombData::EnFeel::Normale)
		{
			m_timeAnimEnd = m_timer;
			m_isLaunch = true;
		}
		else if (m_launceNum >= 3)
		{
			m_timeAnimEnd = m_timer;
			m_isLaunch = true;
		}
		else
		{
			m_timeLaunch = m_timer + m_timeLaunchSpan;
			m_isLaunchSleep = true;

			m_oldIKpos = ikpos;
		}
	}
	return this;
}

void BossBombCoroCoroState::Exit(IEnemy* e)
{
}

void BossBombCoroCoroState::OnAttacked(IEnemy* e)
{
}
