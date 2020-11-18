#include "stdafx.h"
#include "EnemyMeleePunchState.h"
#include "../../EnemyManager.h"
#include "../../IEnemy.h"
#include "../../../Player/Player.h"

EnemyMeleePunchState::EnemyMeleePunchState() {

}

EnemyMeleePunchState::~EnemyMeleePunchState() {

}

void EnemyMeleePunchState::Enter(IEnemy* e) {
	e->PlayAnimation(IEnemy::EnAnimation::enAttackA);
	m_timer = 0.f;
	//e->playAttackAnimation();
	auto player = EnemyManager::GetEnemyManager().GetPlayer();
	player->ApplyDamage(m_damageAmount);

#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Enter enemy dash punch state\n");
#endif
}

IEnemyState* EnemyMeleePunchState::Update(IEnemy* e) {
	m_timer += gameTime()->GetDeltaTime();
	if (m_timer >= m_intervalSec) {
		return e->GetState(IEnemy::EnState::enBattleState);
	}
	return this;
}

void EnemyMeleePunchState::Exit(IEnemy* e) {
#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Exit enemy dush punch state\n");
#endif
}
