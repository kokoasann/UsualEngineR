#include "stdafx.h"
#include "Boss_FatmanTackleState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Boss/Boss_Fatman.h"

Boss_FatmanTackleState::Boss_FatmanTackleState()
{
}

Boss_FatmanTackleState::~Boss_FatmanTackleState()
{
}

void Boss_FatmanTackleState::Enter(IEnemy* e)
{
	m_timer = 0.f;
	
	e->GetModel()->SetRotation(Boss_Fatman::EnemyToPlayerRotation(e, false));

	auto& p = GameManager::GetInstance().m_player;
	const auto& ppos = p->GetPosition();
	m_playerPosition = ppos;
}

IEnemyState* Boss_FatmanTackleState::Update(IEnemy* e)
{
	auto& p = GameManager::GetInstance().m_player;
	const auto& ppos = p->GetPosition();
	const auto& epos = e->GetPosition();
	auto vecToPlayer = ppos - epos;
	float distanceToCurrentP = vecToPlayer.Length();

	const float distance = 5.f;
	if (std::abs(distanceToCurrentP) < distance){
		//プレイヤーがノックバック。
		auto& p = GameManager::GetInstance().m_player;
		const auto& epos = e->GetPosition();
		const auto& ppos = p->GetPosition();
		m_playerPosition = ppos;
		auto vecToPlayer = ppos - epos;
		vecToPlayer.Normalize();
		const float knockbackParam = 100.f;
		vecToPlayer *= knockbackParam;
		p->ApplyDamage(m_damage, true, vecToPlayer);
		return e->GetState(TO_INT(IEnemy::EnState::enBattleState));
	}
	
	auto vecToPlayer2 = m_playerPosition - epos;
	float distanceToOldP = vecToPlayer2.Length();
	if (std::abs(distanceToOldP) < distance) {
		return e->GetState(TO_INT(IEnemy::EnState::enBattleState));
	}

	vecToPlayer2.Normalize();
	const float speed = 80.f;
	Vector3 movespeed = vecToPlayer2 * speed;
	e->SetVelocity(movespeed);
	return this;
}

void Boss_FatmanTackleState::Exit(IEnemy* e)
{
}
