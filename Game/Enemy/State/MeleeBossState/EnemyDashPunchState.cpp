#include "stdafx.h"
#include "EnemyDashPunchState.h"
#include "../../EnemyManager.h"
#include "../../IEnemy.h"
#include "../../../Player/Player.h"

EnemyDashPunchState::EnemyDashPunchState() {

}

EnemyDashPunchState::~EnemyDashPunchState() {

}

void EnemyDashPunchState::Enter(IEnemy* e) {

	e->PlayAnimation(IEnemy::EnAnimation::enIdle);

#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Enter enemy dash punch state\n");
#endif
}

IEnemyState* EnemyDashPunchState::Update(IEnemy* e) {


	return this;
}

void EnemyDashPunchState::Exit(IEnemy* e) {
#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Exit enemy dush punch state\n");
#endif
}
