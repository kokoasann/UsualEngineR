#include "stdafx.h"
#include "EnemyShortRangeComingState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"

EnemyShortRangeComingState::EnemyShortRangeComingState()
{
}

EnemyShortRangeComingState::~EnemyShortRangeComingState()
{
}

void EnemyShortRangeComingState::Enter(IEnemy* e)
{
}

IEnemyState* EnemyShortRangeComingState::Update(IEnemy* e)
{
	auto player = EnemyManager::GetEnemyManager().GetPlayer();
	auto& epos = e->GetPosition();
	auto& ppos = player->GetPosition();
	auto vecToPlayer = ppos - epos;
	const float chaseRange = 50.f;

	if (vecToPlayer.Length() < chaseRange and player->GetCurrentHP() > 0.f) {
		return e->GetState(TO_INT(IEnemy::EnState::enAttackA));
	}

	Move(e);
	return this;
}

void EnemyShortRangeComingState::Exit(IEnemy* e)
{
}

void EnemyShortRangeComingState::Move(IEnemy* e)
{
	auto player = EnemyManager::GetEnemyManager().GetPlayer();
	auto& epos = e->GetPosition();
	auto& ppos = player->GetPosition();
	auto vecToPlayer = ppos - epos;
	vecToPlayer.Normalize();
	
	const float walkSpeed = 50.0f;
	Vector3 moveSpeed = vecToPlayer * walkSpeed;

	e->SetVelocity(moveSpeed);
}
