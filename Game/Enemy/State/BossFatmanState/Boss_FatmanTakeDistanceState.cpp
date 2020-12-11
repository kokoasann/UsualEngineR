#include "stdafx.h"
#include "Boss_FatmanTakeDistanceState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Boss/Boss_Fatman.h"

Boss_FatmanTakeDistanceState::Boss_FatmanTakeDistanceState()
{
}

Boss_FatmanTakeDistanceState::~Boss_FatmanTakeDistanceState()
{
}

void Boss_FatmanTakeDistanceState::Enter(IEnemy* e)
{
	e->GetModel()->SetRotation(Boss_Fatman::EnemyToPlayerRotation(e));
}

IEnemyState* Boss_FatmanTakeDistanceState::Update(IEnemy* e)
{
	
	auto& p = GameManager::GetInstance().m_player;
	const auto& epos = e->GetPosition();
	const auto& ppos = p->GetPosition();
	auto vecToPlayer2 = ppos - epos;
	float t = vecToPlayer2.Length();

	if (t > 100.f) {
		e->SetVelocity(Vector3::Zero);
		return e->GetState(TO_INT(IEnemy::EnState::enBattleState));
	}
	auto vecToPlayer = epos - ppos;
	vecToPlayer.Normalize();
	//vecToPlayer.y = 0.f;
	Vector3 movespeed = vecToPlayer * 100.f;
	e->SetVelocity(movespeed);
	return this;
}

void Boss_FatmanTakeDistanceState::Exit(IEnemy* e)
{
}
