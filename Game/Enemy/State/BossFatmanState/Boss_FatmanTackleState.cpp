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
	e->GetModel()->SetRotation(Boss_Fatman::EnemyToPlayerRotation(e));
	auto& p = GameManager::GetInstance().m_player;
	const auto& ppos = p->GetPosition();
	m_playerPosition = ppos;
}

IEnemyState* Boss_FatmanTackleState::Update(IEnemy* e)
{
	auto& p = GameManager::GetInstance().m_player;
	const auto& epos = e->GetPosition();
	auto vecToPlayer = m_playerPosition - epos;
	float distance = vecToPlayer.Length();

	if (distance < 5.f) {
		//プレイヤーがノックバック。
		/*auto& p = GameManager::GetInstance().m_player;
		const auto& epos = e->GetPosition();
		const auto& ppos = p->GetPosition();
		m_playerPosition = ppos;
		auto vecToPlayer = ppos - epos;
		vecToPlayer.Normalize();
		const float knockbackParam = 100.f;
		vecToPlayer *= knockbackParam;
		p->ApplyDamage(m_damage, true, vecToPlayer);*/
		return e->GetState(TO_INT(IEnemy::EnState::enBattleState));
	}
	
	vecToPlayer.Normalize();
	Vector3 movespeed = vecToPlayer * 20.f;
	e->SetVelocity(movespeed);
	return this;
}

void Boss_FatmanTackleState::Exit(IEnemy* e)
{
}
