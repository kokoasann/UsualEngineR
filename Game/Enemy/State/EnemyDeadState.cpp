#include "stdafx.h"
#include "EnemyDeadState.h"
#include "../EnemyManager.h"

EnemyDeadState::EnemyDeadState() {
}

EnemyDeadState::~EnemyDeadState() {
}

void EnemyDeadState::Enter(IEnemy* e) {
	printf("Enemy - Enter Dead State\n");
	GameObject* enemy = reinterpret_cast<GameObject*>(e);
	EnemyManager::GetEnemyManager().DestroyEnemy(e);
	//DeleteGO(enemy);
	//playAnimation-Dead
}

IEnemyState* EnemyDeadState::Update(IEnemy* e) {
	return this;
}

void EnemyDeadState::Exit(IEnemy* e) {
	printf("Enemy - Exit Dead State\n");
}