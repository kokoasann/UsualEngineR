#include "stdafx.h"
#include "EnemyDeadState.h"
#include "../EnemyManager.h"
#include "../Drop/DropItemA.h"

EnemyDeadState::EnemyDeadState() {
}

EnemyDeadState::~EnemyDeadState() {
}

void EnemyDeadState::Enter(IEnemy* e) {
#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Enemy - Enter Dead State\n");
#endif
	GameObject* enemy = reinterpret_cast<GameObject*>(e);
	EnemyManager::GetEnemyManager().DestroyEnemy(e);

	//Drop Item
	auto dropItemID = e->GetDropID();
	auto item = NewGO<DropItemA>(0);
	item->SetPosition(e->GetPosition());

	//DeleteGO(enemy);
	//playAnimation-Dead
}

IEnemyState* EnemyDeadState::Update(IEnemy* e) {
	return this;
}

void EnemyDeadState::Exit(IEnemy* e) {
#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Enemy - Exit Dead State\n");
#endif
}