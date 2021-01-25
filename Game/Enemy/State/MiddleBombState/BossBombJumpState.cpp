#include "stdafx.h"
#include "BossBombJumpState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Boss/Boss_MiddleBomb.h"
#include "GameManager.h"

BossBombJumpState::BossBombJumpState()
{
}

BossBombJumpState::~BossBombJumpState()
{
}

void BossBombJumpState::Enter(IEnemy* e)
{
	e->PlayAnimation(TO_INT(Boss_MiddleBomb::EnAnimEX::Jump));
	m_isJumpStarted = false;
}

IEnemyState* BossBombJumpState::Update(IEnemy* e)
{
	if (BossBombData::GetInstance().isJumpStart && !m_isJumpStarted)
	{
		m_isJumpStarted = true;
		e->SetVelocity({ 0,300,0 });
	}
	if (m_isJumpStarted)
	{
		e->SetVelocity(e->GetVelocity() * 0.8f);
	}
	const auto& v = e->GetVelocity();
	if (v.y <= 0.1f)
	{
		return e->GetState(TO_INT(Boss_MiddleBomb::EnStateEX::FullFrontal));
	}
	return this;
}

void BossBombJumpState::Exit(IEnemy* e)
{

}

void BossBombJumpState::OnAttacked(IEnemy* e)
{
}
