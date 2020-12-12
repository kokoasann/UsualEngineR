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
	const auto& rot = Boss_Fatman::EnemyToPlayerRotation(e, false);
	e->GetModel()->SetRotation(rot);
	int a = 0;
}

IEnemyState* Boss_FatmanTakeDistanceState::Update(IEnemy* e)
{
	
	auto& p = GameManager::GetInstance().m_player;
	const auto& epos = e->GetPosition();
	const auto& ppos = p->GetPosition();
	auto vecToPlayer = epos - ppos;
	float distance = vecToPlayer.Length();

	if (std::abs(distance) > 100.f){
		e->SetVelocity(Vector3::Zero);
		return e->GetState(TO_INT(IEnemy::EnState::enBattleState));
	}
	
	vecToPlayer.Normalize();
	Vector3 movespeed = vecToPlayer * 100.f;
	e->SetVelocity(movespeed);
	return this;
}

void Boss_FatmanTakeDistanceState::Exit(IEnemy* e)
{
}
