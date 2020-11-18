#include "stdafx.h"
#include "EnemyMeleeBattleState.h"
#include "../../EnemyManager.h"
#include "../../IEnemy.h"
#include "../../../Player/Player.h"
#include "../../Boss/BossA.h"

EnemyMeleeBattleState::EnemyMeleeBattleState() {

}

EnemyMeleeBattleState::~EnemyMeleeBattleState() {

}

void EnemyMeleeBattleState::Enter(IEnemy* e) {

	e->PlayAnimation(IEnemy::EnAnimation::enIdle);

#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Enter enemy melee battle\n");
#endif
}

IEnemyState* EnemyMeleeBattleState::Update(IEnemy* e) {

	auto player = EnemyManager::GetEnemyManager().GetPlayer();

	if (player->GetCurrentHP() <= 0) {
		return e->GetState(TO_INT(BossA::EnState::enIdleState));
	}

	if (player->GetCurrentState() == player->GetState(Player::EnState::enFlying)){
		return e->GetState(TO_INT(BossA::EnState::enFlyState));
	}

	auto& epos = e->GetPosition();
	auto& ppos = player->GetPosition();
	auto vecToPlayer = ppos - epos;
	const float distLimit = 20.f;
	const float attackRange = 30.f;
	const float teleportationDist = 100.f;

	//Teleportation
	if (vecToPlayer.Length() > teleportationDist) {

		auto player = EnemyManager::GetEnemyManager().GetPlayer();
		auto& epos = e->GetPosition();
		auto& ppos = player->GetPosition();
		auto vecToPlayer = ppos - epos;
		auto len = vecToPlayer.Length();
		vecToPlayer.Normalize();
		vecToPlayer *= len * 0.8f;

		auto enextpos = epos + vecToPlayer;

		DebugPrint_WATA("teleportation\n");
		e->SetPosition(enextpos);
		return this;
	}

	//Chase
	if (vecToPlayer.Length() > distLimit) {
		vecToPlayer.Normalize();
		const float speed = 50.f;
		auto velocity = vecToPlayer * speed;

		velocity.y = -10.f;

		e->SetVelocity(velocity);
	}


	if (vecToPlayer.Length() < attackRange) {
		auto rnd = GRandom().Rand();
		if (rnd >= 0.5f) {
			return e->GetState(IEnemy::EnState::enAttackA);
		}
		else {
			return e->GetState(IEnemy::EnState::enAttackB);
		}
	}

	//状態が変わらないなら自分を返す
	return this;
}

void EnemyMeleeBattleState::Exit(IEnemy* e) {
#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Exit enemy melee battle\n");
#endif
}