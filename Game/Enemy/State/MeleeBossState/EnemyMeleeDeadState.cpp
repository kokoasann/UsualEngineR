#include "stdafx.h"
#include "EnemyMeleeDeadState.h"
#include "../../EnemyManager.h"
#include "../../IEnemy.h"
#include "../../../Player/Player.h"
#include "../../Drop/DropItemA.h"

EnemyMeleeDeadState::EnemyMeleeDeadState() {

}

EnemyMeleeDeadState::~EnemyMeleeDeadState() {

}

void EnemyMeleeDeadState::Enter(IEnemy* e) {
#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Enter enemy melee dead\n");
#endif

	auto item = NewGO<DropItemA>(0);
	item->SetPosition(e->GetPosition());

	GameObject* enemy = reinterpret_cast<GameObject*>(e);
	EnemyManager::GetEnemyManager().DestroyEnemy(e);
}

IEnemyState* EnemyMeleeDeadState::Update(IEnemy* e) {
	return this;
}

void EnemyMeleeDeadState::Exit(IEnemy* e) {
#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Exit enemy melee dead\n");
#endif
}