#include "stdafx.h"
#include "PlayerAttackThrowPod.h"
#include "../../Player.h"
#include "../../../Enemy/EnemyManager.h"
#include "../../Pod/Pod.h"

PlayerAttackThrowPod::PlayerAttackThrowPod() {

}

PlayerAttackThrowPod::~PlayerAttackThrowPod() {

}

void PlayerAttackThrowPod::Init(Player* player, int combo) {

	//Idle状態でないなら投げれない.
	if (!player->GetPod()->IsIdle()) {
		m_isDone = true;
		return;
	}

	if (player->GetCurrentEndurance() < m_StaminaCost or player->GetCurrentBoost() < m_BoostCost) {
		m_canDoAttack = false;
		return;
}
	else {
		m_canDoAttack = true;
	}

#ifdef _PRINT_PLAYER_ATTACK
	std::string s = "attack throw-pod combo :" + std::to_string(combo);
	DebugPrint_WATA(s.c_str());
#endif

	player->GetPod()->SetPosition(player->GetPosition());
	auto vel = player->GetForward() * m_throwPower;
	player->GetPod()->Thrown(vel);

	//player->GetPod()->SetPosition();
	m_isDone = false;
	m_timer = 0.f;
	player->PlayAnimation(Player::EnAnimation::enThrow);

	player->UseStamina(m_StaminaCost);
	player->UseBoost(m_BoostCost);

	//auto& enemyManager = EnemyManager::GetEnemyManager();
	//enemyManager.ApplyAoeDamage(/*attack origin*/ player->GetPosition(), m_range, m_damageAmount * combo);
}

void PlayerAttackThrowPod::Execute(Player* player) {

	if (!m_canDoAttack) {
		m_isDone = true;
		return;
	}

	if (!player->IsPlayingAnimation()) {
		m_timer += gameTime()->GetDeltaTime();
	}

	if (m_timer >= m_interval) {
		m_isDone = true;
	}

}


