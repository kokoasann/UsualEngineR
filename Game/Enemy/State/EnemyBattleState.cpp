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
	printf("Enter battle\n");
}

IEnemyState* EnemyBattleState::Update(IEnemy* e) {

	auto player = EnemyManager::GetEnemyManager().GetPlayer();
	if (player->GetCurrentHP() <= 0) {
		return e->GetState(IEnemy::EnState::enIdleState);
	}

	auto& epos = e->GetPosition();
	auto& ppos = player->GetPosition();
	auto vecToPlayer = ppos - epos;
	const float distLimit = 20.f;
	const float attackRange = 30.f;

	if (vecToPlayer.Length() < attackRange) {
		return e->GetState(IEnemy::EnState::enAttackSlash);
	}

	//Chase
	if (vecToPlayer.Length() > distLimit) {
		vecToPlayer.Normalize();
		const float speed = 0.1;
		auto velocity = vecToPlayer * speed;
		auto npos = epos + velocity;
		e->SetPosition(npos);
	}

	//if (g_pad[0]->IsTrigger(enButtonA)) {
		//return e->GetState(IEnemy::EnState::enIdleState);
	//}
	return this;
}

void EnemyBattleState::Exit(IEnemy* e) {
	printf("Exit battle\n");
}