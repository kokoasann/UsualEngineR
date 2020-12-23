#include "stdafx.h"
#include "EnemyShortBigBlown.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Zako/Zako_ShortBig.h"

EnemyShortBigBlown::EnemyShortBigBlown()
{
}

EnemyShortBigBlown::~EnemyShortBigBlown()
{
}

void EnemyShortBigBlown::Enter(IEnemy* e)
{
	e->PlayAnimation(TO_INT(Zako_ShortBig::EnAnimEX::enDown));
	if (m_ikList[0] == nullptr)
	{
		//m_ikList[TO_INT(IEnemy::EnIK::enHead)] = e->GetIK(TO_INT(IEnemy::EnIK::enHead));
		m_ikList[TO_INT(IEnemy::EnIK::enChest)] = e->GetIK(TO_INT(IEnemy::EnIK::enChest));
		m_ikList[TO_INT(IEnemy::EnIK::enArm_L)] = e->GetIK(TO_INT(IEnemy::EnIK::enArm_L));
		m_ikList[TO_INT(IEnemy::EnIK::enArm_R)] = e->GetIK(TO_INT(IEnemy::EnIK::enArm_R));
		m_ikList[TO_INT(IEnemy::EnIK::enFoot_L)] = e->GetIK(TO_INT(IEnemy::EnIK::enFoot_L));
		m_ikList[TO_INT(IEnemy::EnIK::enFoot_R)] = e->GetIK(TO_INT(IEnemy::EnIK::enFoot_R));

		for (auto ik : m_ikList)
		{
			if (ik == nullptr)
				continue;
			ik->SetIKMode(IK::enMode_NoAnimHit);
		}
	}

	m_velocityXZ = e->GetKnockBackImpulse();
	m_velocityY = 0.f;
	m_isNoBlown = false;
	if (m_velocityXZ.y != 0.f)
		m_velocityY = max(m_velocityXZ.y / 5.f, 120.f);
	else
	{
		m_grav = 5.f;
		m_isNoBlown = true;
	}
	m_velocityXZ.y = 0.f;
	m_velocityXZ *= 0.1f;
	float xzLen = m_velocityXZ.Length();
	if (xzLen == 0.f)
	{
		m_velocityXZ = Vector3::Zero;
	}
	else if (xzLen < 60)
	{
		m_velocityXZ /= xzLen;
		//’Ç‰Á
		m_velocityXZ *= (60.f * m_bigPush);
	}
	m_timer = 0.f;
	m_isTakeOff = m_isNoBlown;
	m_isTakeOn = false;

	e->RemoveRigidBody();
}

IEnemyState* EnemyShortBigBlown::Update(IEnemy* e)
{
	if (m_timer >= m_timeLimit)
	{

	}
	float dtime = gameTime()->GetDeltaTime();


	auto ve = Vector3{ m_velocityXZ.x ,m_velocityY ,m_velocityXZ.z };
	//auto ve = Vector3{ m_velocityXZ.x ,0,m_velocityXZ.z };

	{
		//m_ikList[TO_INT(IEnemy::EnIK::enHead)]->SetVelocity(ve * 2.f);
		if (!m_ikList[TO_INT(IEnemy::EnIK::enChest)]->IsHit())
			m_ikList[TO_INT(IEnemy::EnIK::enChest)]->SetVelocity(ve * 1.f);

		if (!m_ikList[TO_INT(IEnemy::EnIK::enArm_L)]->IsHit())
			m_ikList[TO_INT(IEnemy::EnIK::enArm_L)]->SetVelocity(ve * 1.f);

		if (!m_ikList[TO_INT(IEnemy::EnIK::enArm_R)]->IsHit())
			m_ikList[TO_INT(IEnemy::EnIK::enArm_R)]->SetVelocity(ve * 1.f);

		auto vefoot = Vector3(-ve.x, ve.y, -ve.z);
		vefoot *= 0.5f;
		if (!m_ikList[TO_INT(IEnemy::EnIK::enFoot_L)]->IsHit())
			m_ikList[TO_INT(IEnemy::EnIK::enFoot_L)]->SetVelocity(vefoot);

		if (!m_ikList[TO_INT(IEnemy::EnIK::enFoot_R)]->IsHit())
			m_ikList[TO_INT(IEnemy::EnIK::enFoot_R)]->SetVelocity(vefoot);
	}
	/*for (auto ik : m_ikList)
	{
		ik->SetVelocity(ve);

	}*/
	if (m_isTakeOff && e->IsOnGround())
	{
		m_isTakeOn = true;
		e->SetExternalVelocity(Vector3::Zero);
	}
	if (!m_isTakeOn || (!m_ikList[TO_INT(IEnemy::EnIK::enChest)]->IsHit() && m_isNoBlown))
		e->SetExternalVelocity(ve);


	if (!m_isTakeOff && !e->IsOnGround())
		m_isTakeOff = true;


	m_velocityY -= m_grav * dtime;
	//m_timer += dtime;

	return this;
}

void EnemyShortBigBlown::Exit(IEnemy* e)
{
}

void EnemyShortBigBlown::OnAttacked(IEnemy* e)
{
}
