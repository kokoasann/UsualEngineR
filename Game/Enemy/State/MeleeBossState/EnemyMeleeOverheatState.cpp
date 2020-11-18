#include "stdafx.h"
#include "EnemyMeleeOverheatState.h"
#include "../../EnemyManager.h"
#include "../../IEnemy.h"
#include "../../../Player/Player.h"
#include "../../Boss/BossA.h"

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

	const float GRAVITY = -50.f;

	m_timer += gameTime()->GetDeltaTime();

	auto vel = Vector3::Zero;
	vel.y = GRAVITY;
	e->SetVelocity(vel);

	if (m_timer > m_time and e->IsOnGround()) {
		return e->GetStateList().at(TO_INT(BossA::EnState::enBattleState));
	}

	return this;
}

void EnemyMeleeOverheatState::Exit(IEnemy* e) {
#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Exit enemy overheat\n");
#endif
}