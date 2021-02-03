#include "stdafx.h"
#include "BossBombRollingState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"

#include "Enemy/Boss/Boss_MiddleBomb.h"
#include "GameManager.h"

BossBombRollingState::BossBombRollingState()
{
}

BossBombRollingState::~BossBombRollingState()
{
}

void BossBombRollingState::Enter(IEnemy* e)
{
	e->PlayAnimation(TO_INT(Boss_MiddleBomb::EnAnimEX::Guard));
	
	m_timer = 0.f;
}

IEnemyState* BossBombRollingState::Update(IEnemy* e)
{
	float dtime = gameTime()->GetDeltaTime();
	m_timer += dtime;
	if (m_timer >= m_timeLimit)
		return e->GetState(TO_INT(IEnemy::EnState::enBattleState));

	const auto& rot = e->GetRotation();

	Quaternion addrot;
	addrot.SetRotationDegY(360.f * dtime * 5.f);

	e->SetRotation(rot * addrot);

	auto p = GameManager::GetInstance().GetPlayer();
	auto e2p = p->GetPosition() - e->GetPosition();
	float e2pLen = e2p.Length();
	if (e2pLen < 20.f)
	{
		e2p.Normalize();
		p->ApplyDamage(15, true, e2p * 200.f);
	}

	return this;
}

void BossBombRollingState::Exit(IEnemy* e)
{
}

void BossBombRollingState::OnAttacked(IEnemy* e)
{
}
