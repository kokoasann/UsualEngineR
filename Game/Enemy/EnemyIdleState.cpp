#include "stdafx.h"
#include "EnemyIdleState.h"

EnemyIdleState::EnemyIdleState() {

}

EnemyIdleState::~EnemyIdleState() {

}

void EnemyIdleState::Enter(IEnemy* e) {
	printf("Enter\n");
}

IEnemyState* EnemyIdleState::Update(IEnemy* e) {
	return this;
}

void EnemyIdleState::Exit(IEnemy* e) {
	printf("Exit");
}