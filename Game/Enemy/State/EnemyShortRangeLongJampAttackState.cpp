#include "stdafx.h"
#include "EnemyShortRangeLongJampAttackState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"

#include "Enemy/Zako/Zako_ShortRangeMonster.h"

EnemyShortRangeLongJampAttackState::EnemyShortRangeLongJampAttackState()
{
}

EnemyShortRangeLongJampAttackState::~EnemyShortRangeLongJampAttackState()
{
}

void EnemyShortRangeLongJampAttackState::Enter(IEnemy* e)
{
	e->PlayAnimation(IEnemy::EnAnimation::enAttackB);
	m_timer = 0.f;
	m_foward = GameManager::GetInstance().m_player->GetPosition() - e->GetPosition();
	m_foward.y = 0;
	m_foward.Normalize();
	m_foward *= m_fowardMove;
	m_isTakOff = false;
	m_gravPow = m_jampHeight;
	m_isApplyDamage = false;
}

IEnemyState* EnemyShortRangeLongJampAttackState::Update(IEnemy* e)
{
	
	if (m_isTakOff && e->IsOnGround())
	{
		e->SetVelocity(Vector3::Zero);
		return e->GetState(TO_INT(IEnemy::EnState::enBattleState));
	}
	
	float dtime = gameTime()->GetDeltaTime();
	m_timer += dtime;
	//auto up = Vector3::Up;
	
	float t = m_timer / m_timeLimit;
	m_gravPow -= /*(t - t * t) * 4.f * */m_grav * dtime;
	Vector3 up = { 0,m_gravPow,0 };

	
	e->SetVelocity(up + m_foward);

	if (!m_isApplyDamage)
	{
		auto p = GameManager::GetInstance().GetPlayer();
		auto e2p = p->GetPosition() - e->GetPosition();
		if (e2p.Length() < 10.f)
		{
			e2p = m_foward;
			e2p.Normalize();
			p->ApplyDamage(m_damage, true, e2p * 150);
			m_isApplyDamage = true;
		}
	}

	//if (e->IsOnGround())
		//e->SetVelocity(Vector3::Zero);

	if(!m_isTakOff)
		//m_isTakOff = t>=0.5f;
		m_isTakOff = !e->IsOnGround();
	
	return this;
}

void EnemyShortRangeLongJampAttackState::Exit(IEnemy* e)
{
}
