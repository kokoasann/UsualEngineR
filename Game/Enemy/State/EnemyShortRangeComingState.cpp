#include "stdafx.h"
#include "EnemyShortRangeComingState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Zako/Zako_ShortRangeMonster.h"

EnemyShortRangeComingState::EnemyShortRangeComingState()
{
}

EnemyShortRangeComingState::~EnemyShortRangeComingState()
{
}

void EnemyShortRangeComingState::Enter(IEnemy* e)
{
	e->PlayAnimation(IEnemy::EnAnimation::enWalk);
}

IEnemyState* EnemyShortRangeComingState::Update(IEnemy* e)
{
	auto player = EnemyManager::GetEnemyManager().GetPlayer();
	auto& epos = e->GetPosition();
	auto& ppos = player->GetPosition();
	auto vecToPlayer = ppos - epos;
	const float moveRange = 10.f;
	const float attackRange = 10.f;
	const float idleRange = 300.f;

	if (player->GetCurrentHP() > 0.f) {
		//近づいたら行動。
		if (vecToPlayer.Length() < attackRange) {
			auto rand = GRandom().Rand();
			//約30%の確率で行動変化。
			if (rand < 0.33f) {
				//パンチ。
				return e->GetState(TO_INT(IEnemy::EnState::enAttackA));
			}
			else if (rand < 0.66f) {
				//大振りのパンチ。
				return e->GetState(TO_INT(IEnemy::EnState::enAttackB));
			}
			else {
				//ダンス。
				return e->GetState(TO_INT(Zako_ShortRangeMonster::EnStateEX::enDance));
			}
		}
		//離れたら移動。
		else {
			Move(e);
		}

		//離れたら移動停止。
		if (vecToPlayer.Length() > idleRange) {
			return e->GetState(TO_INT(IEnemy::EnState::enIdleState));
		}
	}

	return this;
}

void EnemyShortRangeComingState::Exit(IEnemy* e)
{
	e->SetVelocity(Vector3::Zero);
}

void EnemyShortRangeComingState::Move(IEnemy* e)
{
	auto player = EnemyManager::GetEnemyManager().GetPlayer();
	auto& epos = e->GetPosition();
	auto& ppos = player->GetPosition();
	auto vecToPlayer = ppos - epos;
	vecToPlayer.Normalize();
	
	const float walkSpeed = 40.0f;
	Vector3 moveSpeed = vecToPlayer * walkSpeed;

	//重力。
	moveSpeed.y -= 100.0f;
	
	e->SetVelocity(moveSpeed);
}

bool EnemyShortRangeComingState::Animation()
{
	return true;
}
