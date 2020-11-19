#include "stdafx.h"
#include "EnemyMeleeIdleState.h"
#include "../../EnemyManager.h"
#include "../../IEnemy.h"
#include "../../../Player/Player.h"
#include "../../Boss/BossA.h"

EnemyMeleeIdleState::EnemyMeleeIdleState() {

}

EnemyMeleeIdleState::~EnemyMeleeIdleState() {

}

void EnemyMeleeIdleState::Enter(IEnemy* e) {

	e->PlayAnimation(IEnemy::EnAnimation::enIdle);

	e->SetVelocity(Vector3::Zero);

#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Enter enemy melee idle\n");
#endif
}

IEnemyState* EnemyMeleeIdleState::Update(IEnemy* e) {

	auto player = EnemyManager::GetEnemyManager().GetPlayer();
	auto& epos = e->GetPosition();
	auto& ppos = player->GetPosition();
	auto vecToPlayer = ppos - epos;
	const float chaseRange = 60.f;

	const float GRAVITY = -50.f;
	auto vel = Vector3::Zero;
	vel.y = GRAVITY;
	//if(!e->IsOnGround())
//		e->SetVelocity(vel);

	if (vecToPlayer.Length() < chaseRange and player->GetCurrentHP() > 0.f) {
		return e->GetStateList().at(TO_INT(BossA::EnState::enBattleState));
	}

	return this;
}

void EnemyMeleeIdleState::Exit(IEnemy* e) {
#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Exit enemy melee idle\n");
#endif
}