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
	float dtime = gameTime()->GetDeltaTime();
	float grav = 50.f;
	if (BossBombData::GetInstance().isJumpStart && !m_isJumpStarted)
	{
		m_isJumpStarted = true;
		const auto& ppos = GameManager::GetInstance().GetPlayer()->GetPosition();
		const auto& epos = e->GetPosition();
		float y = ppos.y - epos.y;
		if (y > 0.f)
		{
			float f = floorf(y / grav);
			if (f == 0.f)
				return e->GetState(TO_INT(IEnemy::EnState::enBattleState));
			float jump = min(y + grav * dtime * f,200.f);

			e->SetVelocity({ 0,jump,0 });
		}
		else
		{
			e->SetVelocity({ 0,30,0 });
		}
		e->SetExternalVelocity(Vector3::Zero);
	}
	if (m_isJumpStarted)
	{
		e->SetVelocity(e->GetVelocity() - Vector3::Up * (grav * dtime));
	}
	const auto& v = e->GetVelocity();
	if (m_isJumpStarted && v.y <= 0.1f)
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
