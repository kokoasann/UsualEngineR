#include "stdafx.h"
#include "PlayerAttackKick.h"
#include "../../Player.h"
#include "../../../Enemy/EnemyManager.h"

PlayerAttackKick::PlayerAttackKick() {

}

PlayerAttackKick::~PlayerAttackKick() {

}

void PlayerAttackKick::Init(Player* player, int combo) {
#ifdef _PRINT_PLAYER_ATTACK
	std::string s = "attack Kick combo :" + std::to_string(combo);
	DebugPrint_WATA(s.c_str());
#endif
	
	if (player->GetCurrentEndurance() < m_StaminaCost or player->GetCurrentBoost() < m_BoostCost) {
		m_canDoAttack = false;
		return;
	}
	else {
		m_canDoAttack = true;
	}

	m_isDone = false;
	m_timer = 0.f;
	player->PlayAnimation(Player::EnAnimation::enKick);

	auto& enemyManager = EnemyManager::GetEnemyManager();
	//enemyManager.ApplyAoeDamage(/*attack origin*/ player->GetPosition(), m_range, m_damageAmount * combo);

	player->UseStamina(m_StaminaCost);
	player->UseBoost(m_BoostCost);
	m_hasAlreadyAttacked = false;

	m_combo = combo;

	player->FireThrusters();

	m_attackedEnemyMap.clear();
}

void PlayerAttackKick::Execute(Player* player) {
	if (!m_canDoAttack) {
		m_isDone = true;
		return;
	}
	if(!player->IsPlayingAnimation()){
		m_timer += gameTime()->GetDeltaTime();
	}
	else {
		if (player->ColCheck(Player::EnPlayerBone::enSOLE_R)) {
			auto& enemyManager = EnemyManager::GetEnemyManager();
			auto nearestEnemy = enemyManager.GetNearestEnemy(player->GetBone(Player::EnPlayerBone::enSOLE_R)->GetWorldMatrix().GetTransrate());
			if (m_attackedEnemyMap.find(nearestEnemy) == m_attackedEnemyMap.end()) {
				nearestEnemy->ApplyDamage(m_damageAmount * m_combo);
				m_attackedEnemyMap.insert(std::make_pair(nearestEnemy, true));
			}
			//m_hasAlreadyAttacked = true;
		}
	}

	if (m_timer >= m_interval) {
		m_isDone = true;
	}
}

