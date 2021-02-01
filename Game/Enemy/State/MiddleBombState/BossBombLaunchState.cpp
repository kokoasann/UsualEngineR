#include "stdafx.h"
#include "BossBombLaunchState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Equipment/Enemy_LaunchBomb.h"
#include "Enemy/Boss/Boss_MiddleBomb.h"
#include "GameManager.h"

#include "Effect/MuzzleFlash.h"

BossBombLaunchState::BossBombLaunchState():
	m_damage(Player::GetMaxHP()/15.f)
{
	m_effect = NewGO<MuzzleFlash>(0);
	MuzzleFlashEffectInitData eid;
	
	m_effect->Init(eid);
}

BossBombLaunchState::~BossBombLaunchState()
{
	DeleteGO(m_effect);
}

void BossBombLaunchState::Enter(IEnemy* e)
{
	e->PlayAnimation(TO_INT(IEnemy::EnAnimation::enAttackA));

	auto ik = e->GetIK(TO_INT(IEnemy::EnIK::enArm_L));
	auto bone = ik->GetEffectorBone();
	if(!m_cannonBone)
		m_cannonBone = e->GetModel()->GetModel().GetSkelton()->GetBone(bone->GetParentBoneNo());
	e->SetAutoRotateFlag(false);

	m_firstRot = e->GetRotation();
	m_timer = 0.f;
	m_isLaunch = false;

	m_launceNum = 0;
	m_timeLaunch = 0.f;
	m_isLaunchSleep = false;
}

IEnemyState* BossBombLaunchState::Update(IEnemy* e)
{
	m_timer += gameTime()->GetDeltaTime();
	float t = m_timer / m_timeLimit;
	t = min(t, 1.f);

	auto ik = e->GetIK(TO_INT(IEnemy::EnIK::enArm_L));
	auto bone = ik->GetEffectorBone();

	auto e2p = GameManager::GetInstance().GetPlayer()->GetPosition() - e->GetPosition();

	if (BossBombData::GetInstance().feeling == BossBombData::EnFeel::Normale)
	{
		ik->SetNextTarget(GameManager::GetInstance().GetPlayer()->GetPosition());
	}
	else
	{
		switch (m_launceNum)
		{
		case 0:
			ik->SetNextTarget(GameManager::GetInstance().GetPlayer()->GetPosition());
			break;
		case 1:
		{
			auto t = min((m_timer - (m_timeLaunch - m_timeLaunchSpan)) / m_timeLaunchSpan, 1.f);
			Quaternion rot;
			rot.SetRotationDegY(5.f);
			auto ikpos = e2p;
			rot.Apply(ikpos);
			ikpos += e->GetPosition();
			ikpos.Lerp(t, m_oldIKpos, ikpos);
			ik->SetNextTarget(ikpos);
		}
			break;
		}
	}
	
	e2p.y = 0.;
	e2p.Normalize();
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
		//if (BossBombData::GetInstance().feeling == BossBombData::EnFeel::Normale)
			return this;
	}

	if ((m_timer - m_timeAnimEnd) - m_timeEnd >= 0.f && m_isLaunch)
		return e->GetState(TO_INT(Boss_MiddleBomb::EnStateEX::Guard));

	
	if (!m_isLaunch)
	{
		const auto& mat = bone->GetWorldMatrix();
		const auto& pmat = m_cannonBone->GetWorldMatrix();

		Enemy_LaunchBomb* bomb = NewGO<Enemy_LaunchBomb>(0, true);
		Vector3 v = Vector3::Up;
		pmat.GetRotate().Apply(v);
		//v.y += 0.3;
		auto bonePos = mat.GetTransrate();
		bomb->Init(bonePos, v * 500., 1, 20, m_damage, 25);
		CSoundSource* se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/shoot.wav");
		se->SetVolume(1.);
		se->Play(false);

		m_effect->Play();
		m_effect->SetPos(bonePos);
		m_effect->SetRot(pmat.GetRotate());
		
		m_launceNum++;
		if (BossBombData::GetInstance().feeling == BossBombData::EnFeel::Normale)
		{
			m_timeAnimEnd = m_timer;
			m_isLaunch = true;
		}
		else if (m_launceNum >= 2)
		{
			m_timeAnimEnd = m_timer;
			m_isLaunch = true;
		}
		else
		{
			m_timeLaunch = m_timer + m_timeLaunchSpan;
			m_isLaunchSleep = true;

			m_oldIKpos = GameManager::GetInstance().GetPlayer()->GetPosition();
		}
	}

	return this;
	return e->GetState(TO_INT(Boss_MiddleBomb::EnStateEX::Guard));
	//return e->GetState(TO_INT(IEnemy::EnState::enBattleState));
}

void BossBombLaunchState::Exit(IEnemy* e)
{
	e->SetVelocity(Vector3::Zero);
}

void BossBombLaunchState::OnAttacked(IEnemy* e)
{
}
