#include "stdafx.h"
#include "EnemyDashPunchState.h"
#include "../../EnemyManager.h"
#include "../../IEnemy.h"
#include "../../../Player/Player.h"

EnemyDashPunchState::EnemyDashPunchState() {

}

EnemyDashPunchState::~EnemyDashPunchState() {

}

void EnemyDashPunchState::Enter(IEnemy* e) {

	e->PlayAnimation(IEnemy::EnAnimation::enAttackB);
	m_timer = 0.f;
	//e->playAttackAnimation();
	auto player = EnemyManager::GetEnemyManager().GetPlayer();
	player->ApplyDamage(m_damageAmount);

#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Enter enemy dash punch state\n");
#endif
}

IEnemyState* EnemyDashPunchState::Update(IEnemy* e) {

	auto player = EnemyManager::GetEnemyManager().GetPlayer();
	auto& epos = e->GetPosition();
	auto& ppos = player->GetPosition();
	auto vecToPlayer = ppos - epos;

	m_timer += gameTime()->GetDeltaTime();
	if (m_timer >= m_intervalSec) {
		return e->GetState(IEnemy::EnState::enBattleState);
	}
	return this;
}

void EnemyDashPunchState::Exit(IEnemy* e) {
#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Exit enemy dush punch state\n");
#endif
}
