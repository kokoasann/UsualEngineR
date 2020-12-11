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

	//プレイヤーがノックバック。
	auto& p = GameManager::GetInstance().m_player;
	const auto& epos = e->GetPosition();
	const auto& ppos = p->GetPosition();
	auto vecToPlayer = ppos - epos;
	vecToPlayer.Normalize();
	const float knockbackParam = 100.f;
	vecToPlayer *= knockbackParam;
	//vecToPlayer.y = 150.f;
	p->ApplyDamage(m_damage, true, vecToPlayer);
}

IEnemyState* Boss_FatmanTackleState::Update(IEnemy* e)
{
	const float intervalSec = 1.f;
	m_timer += gameTime()->GetDeltaTime();
	if (m_timer >= intervalSec) {
		return e->GetState(TO_INT(IEnemy::EnState::enBattleState));
	}
	return this;
}

void Boss_FatmanTackleState::Exit(IEnemy* e)
{
}
