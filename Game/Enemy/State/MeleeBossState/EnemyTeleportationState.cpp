#include "stdafx.h"
#include "EnemyTeleportationState.h"
#include "../../EnemyManager.h"
#include "../../IEnemy.h"
#include "../../../Player/Player.h"

EnemyTeleportationState::EnemyTeleportationState() {

}

EnemyTeleportationState::~EnemyTeleportationState() {

}

void EnemyTeleportationState::Enter(IEnemy* e) {

	e->PlayAnimation(IEnemy::EnAnimation::enIdle);

#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Enter enemy teleportation\n");
#endif
}

IEnemyState* EnemyTeleportationState::Update(IEnemy* e) {

	auto player = EnemyManager::GetEnemyManager().GetPlayer();
	auto& epos = e->GetPosition();
	auto& ppos = player->GetPosition();
	auto vecToPlayer = ppos - epos;
	auto len = vecToPlayer.Length();
	vecToPlayer.Normalize();
	vecToPlayer *= len * 0.8f;

	auto enextpos = epos + vecToPlayer;
	e->SetPosition(enextpos);

	return e->GetStateList().at(TO_INT(IEnemy::EnState::enBattleState));
}

void EnemyTeleportationState::Exit(IEnemy* e) {
#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Exit enemy teleportation\n");
#endif
}