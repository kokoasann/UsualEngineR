#include "stdafx.h"
#include "EnemyShortRangeLongJampAttackState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"

EnemyShortRangeLongJampAttackState::EnemyShortRangeLongJampAttackState()
{
}

EnemyShortRangeLongJampAttackState::~EnemyShortRangeLongJampAttackState()
{
}

void EnemyShortRangeLongJampAttackState::Enter(IEnemy* e)
{
	m_timer = 0.f;
	m_foward = GameManager::GetInstance().m_player->GetPosition() - e->GetPosition();
	m_foward.Normalize();
	m_foward *= m_fowardMove;
	m_isTakOff = false;
}

IEnemyState* EnemyShortRangeLongJampAttackState::Update(IEnemy* e)
{
	
	if (m_isTakOff && e->IsOnGround())
	{

	}

	m_timer += gameTime()->GetDeltaTime();
	auto up = Vector3::Up;
	
	float t = m_timer / m_timeLimit;
	up *= (t - t * t) * 4.f * m_jampHeight;
	
	e->SetVelocity(up + m_foward);

	if(!m_isTakOff)
		m_isTakOff = e->IsOnGround();
	
	return this;
}

void EnemyShortRangeLongJampAttackState::Exit(IEnemy* e)
{
}
