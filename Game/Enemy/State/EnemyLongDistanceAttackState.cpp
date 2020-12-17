#include "stdafx.h"
#include "Enemy/Zako/Zako_LongDistanceMachine.h"
#include "EnemyLongDistanceAttackState.h"
#include "EnemyLongDistanceTargetingState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/Equipment/Enemy_Bullet.h"
#include "Effect/MuzzleFlash.h"

EnemyLongDistanceAttackState::~EnemyLongDistanceAttackState()
{
	DeleteGO(m_muzzleFlash);
}

void EnemyLongDistanceAttackState::Init(IK* ik, float bulletSpeed, EnemyLongDistanceTargetingState* ldt)
{
	m_ik = ik;
	m_speed = bulletSpeed;
	m_ldt = ldt;
	m_muzzleFlash = NewGO<MuzzleFlash>(0);
	MuzzleFlashEffectInitData mfid;
	m_muzzleFlash->Init(mfid);
	m_muzzleFlash->SetSca(Vector3::One * 0.1);
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

	if (m_timer > 2.f)
		return m_ldt;
	//ŽžŠÔ‚É‚È‚Á‚½‚ç’e‚ð”­ŽËB
	if (m_timer >= m_timeSpan && m_shotCount < m_maxShot)
	{
		Enemy_Bullet* eb = NewGO<Enemy_Bullet>(0,true);
		auto bone = m_ik->GetEffectorBone();
		const auto& mat = bone->GetWorldMatrix();
		auto p = mat.GetTransrate();
		auto rot = mat.GetRotate();

		m_muzzleFlash->Play();
		m_muzzleFlash->SetPos(p);
		m_muzzleFlash->SetRot(rot);
		
		
		//p.y += 10.;
		eb->Init(p, 0.25f, m_velocity, m_speed, 5, 1);
		eb->SetDamage(Player::GetMaxHP() / 60.f / 3.f);
		m_timer = 0.f;
		m_shotCount++;
		if (m_shotCount >= m_maxShot)
		{
			//return m_ldt;
		}
	}
	
	return this;
}

void EnemyLongDistanceAttackState::Exit(IEnemy* e)
{
}
