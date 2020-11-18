#include "stdafx.h"
#include "EnemyMeleeOverheatState.h"
#include "../../EnemyManager.h"
#include "../../IEnemy.h"
#include "../../../Player/Player.h"

EnemyMeleeOverheatState::EnemyMeleeOverheatState() {

}

EnemyMeleeOverheatState::~EnemyMeleeOverheatState() {

}

void EnemyMeleeOverheatState::Enter(IEnemy* e) {

	e->PlayAnimation(IEnemy::EnAnimation::enIdle);

#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Enter enemy overheat\n");
#endif
}

IEnemyState* EnemyMeleeOverheatState::Update(IEnemy* e) {

	m_timer += gameTime()->GetDeltaTime();

	if (m_timer > m_time) {
		return e->GetStateList().at(TO_INT(IEnemy::EnState::enBattleState));
	}

	return this;
}

void EnemyMeleeOverheatState::Exit(IEnemy* e) {
#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Exit enemy overheat\n");
#endif
}