#include "stdafx.h"
#include "EnemyIdleState.h"
#include "IEnemy.h"

EnemyIdleState::EnemyIdleState() {

}

EnemyIdleState::~EnemyIdleState() {

}

void EnemyIdleState::Enter(IEnemy* e) {
	printf("Enter enemy Idle\n");
}

IEnemyState* EnemyIdleState::Update(IEnemy* e) {
	if (g_pad[0]->IsTrigger(enButtonA)) {
		return e->GetState(IEnemy::EnState::enBattleState);
	}
	return this;
}

void EnemyIdleState::Exit(IEnemy* e) {
	printf("Exit enemy Idle\n");
}