#include "stdafx.h"
#include "EnemyBattleState.h"
#include "IEnemy.h"

EnemyBattleState::EnemyBattleState() {
}

EnemyBattleState::~EnemyBattleState() {
}

void EnemyBattleState::Enter(IEnemy* e) {
	printf("Enter battle\n");
}

IEnemyState* EnemyBattleState::Update(IEnemy* e) {
	if (g_pad[0]->IsTrigger(enButtonA)) {
		return e->GetState(IEnemy::EnState::enIdleState);
	}
	return this;
}

void EnemyBattleState::Exit(IEnemy* e) {
	printf("Exit battle\n");
}