#include "stdafx.h"
#include "EnemyBattleState.h"
#include "../IEnemy.h"
#include "../EnemyManager.h"
#include "../../Player/Player.h"

EnemyBattleState::EnemyBattleState() {
}

EnemyBattleState::~EnemyBattleState() {
}

void EnemyBattleState::Enter(IEnemy* e) {

	e->PlayAnimation(IEnemy::EnAnimation::enRun);

#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Enter battle\n");
#endif // _PRINT_ENEMY_STATE
}

IEnemyState* EnemyBattleState::Update(IEnemy* e) {

	auto player = EnemyManager::GetEnemyManager().GetPlayer();

	if (player->GetCurrentHP() <= 0) {
		return e->GetState(TO_INT(IEnemy::EnState::enIdleState));
	}

	auto& epos = e->GetPosition();
	auto& ppos = player->GetPosition();
	auto vecToPlayer = ppos - epos;
	const float distLimit = 20.f;
	const float attackRange = 30.f;

	if (vecToPlayer.Length() < attackRange) {
		return e->GetState(TO_INT(IEnemy::EnState::enAttackA));
	}

	//Chase
	if (vecToPlayer.Length() > distLimit) {
		vecToPlayer.Normalize();
		const float speed = 50.f;
		auto velocity = vecToPlayer * speed;

		velocity.y = -10.f;

		e->SetVelocity(velocity);
	}
	//if (g_pad[0]->IsTrigger(enButtonA)) {
		//return e->GetState(IEnemy::EnState::enIdleState);
	//}
	return this;
}

void EnemyBattleState::Exit(IEnemy* e) {
#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Exit battle\n");
#endif
}