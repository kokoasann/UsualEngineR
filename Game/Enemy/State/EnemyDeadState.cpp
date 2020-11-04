#include "stdafx.h"
#include "EnemyDeadState.h"
#include "../EnemyManager.h"

EnemyDeadState::EnemyDeadState() {
}

EnemyDeadState::~EnemyDeadState() {
}

void EnemyDeadState::Enter(IEnemy* e) {
#ifdef _PRINT_ENEMY_STATE
	printf("Enemy - Enter Dead State\n");
#endif
	GameObject* enemy = reinterpret_cast<GameObject*>(e);
	EnemyManager::GetEnemyManager().DestroyEnemy(e);
	//DeleteGO(enemy);
	//playAnimation-Dead
}

IEnemyState* EnemyDeadState::Update(IEnemy* e) {
	return this;
}

void EnemyDeadState::Exit(IEnemy* e) {
#ifdef _PRINT_ENEMY_STATE
	printf("Enemy - Exit Dead State\n");
#endif
}