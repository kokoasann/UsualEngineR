#include "stdafx.h"
#include "EnemyMeleePunchState.h"
#include "../../EnemyManager.h"
#include "../../IEnemy.h"
#include "../../../Player/Player.h"
#include "../../Boss/BossA.h"

EnemyMeleePunchState::EnemyMeleePunchState() {

}

EnemyMeleePunchState::~EnemyMeleePunchState() {

}

void EnemyMeleePunchState::Enter(IEnemy* e) {

	m_canExecute = true;

	if (e->GetAbility().stamina < m_cost) {
		m_canExecute = false;
		return;
	}

	e->PlayAnimation(IEnemy::EnAnimation::enAttackA);

	e->UseStamina(m_cost);

	m_timer = 0.f;
	//e->playAttackAnimation();
	auto player = GameManager::GetInstance().GetPlayer();

	player->ApplyDamage(m_damageAmount);

#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Enter enemy dash punch state\n");
#endif
}

IEnemyState* EnemyMeleePunchState::Update(IEnemy* e) {

	if (!m_canExecute) {
		return e->GetState(TO_INT(IEnemy::EnState::enBattleState));
	}

	m_timer += gameTime()->GetDeltaTime();
	if (m_timer >= m_intervalSec) {
		return e->GetState(TO_INT(IEnemy::EnState::enBattleState));
	}
	return this;
}

void EnemyMeleePunchState::Exit(IEnemy* e) {
#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Exit enemy dush punch state\n");
#endif
}
