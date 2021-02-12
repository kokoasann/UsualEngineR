#include "stdafx.h"
#include "EnemyMeleeDeadState.h"
#include "../../EnemyManager.h"
#include "../../IEnemy.h"
#include "../../../Player/Player.h"
#include "../../Drop/DropItemA.h"
#include "GameManager.h"
#include "Game.h"

EnemyMeleeDeadState::EnemyMeleeDeadState() {

}

EnemyMeleeDeadState::~EnemyMeleeDeadState() {
}

void EnemyMeleeDeadState::Enter(IEnemy* e) {

#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Enter enemy melee dead\n");
#endif

	m_pos = e->GetPosition();
	//m_pos.y += 15.f;

	if (!m_isPerformed) {
		e->GetModel()->SetMulColor({ 20,1,1,1 });
		GameManager::GetInstance().m_gameScene->OnEnemyDied(e);
		m_isPerformed = true;
	}

	//GameObject* enemy = reinterpret_cast<GameObject*>(e);
	//EnemyManager::GetEnemyManager().DestroyEnemy(e);
}

IEnemyState* EnemyMeleeDeadState::Update(IEnemy* e) {
	return this;
}

void EnemyMeleeDeadState::Exit(IEnemy* e) {
#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Exit enemy melee dead\n");
#endif
}