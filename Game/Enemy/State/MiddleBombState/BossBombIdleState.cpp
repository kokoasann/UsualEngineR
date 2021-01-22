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
}

IEnemyState* BossBombIdleState::Update(IEnemy* e)
{
	if(m_isAttacked)
		return e->GetState(TO_INT(IEnemy::EnState::enBattleState));

	e->SetExternalVelocity({ 0,-500,0 });
	
	auto p = GameManager::GetInstance().GetPlayer();
	auto d = e->GetPosition() - p->GetPosition();
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
