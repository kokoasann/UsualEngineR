#include "stdafx.h"
#include "PlayerAttackRevolvingTackle.h"
#include "../../Player.h"
#include "../../../Enemy/EnemyManager.h"

PlayerAttackRevolvingTackle::PlayerAttackRevolvingTackle() {

}

PlayerAttackRevolvingTackle::~PlayerAttackRevolvingTackle() {

}

void PlayerAttackRevolvingTackle::Init(Player* player, int combo) {
#ifdef _PRINT_PLAYER_ATTACK
	std::string s = "attack revolving tackle combo :" + std::to_string(combo);
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
	m_isContinuAttack = false;
	m_timer = 0.f;
	player->PlayAnimation(Player::EnAnimation::enRevolvingTackle);

	player->FireThrusters();

	player->UseStamina(m_StaminaCost);
	player->UseBoost(m_BoostCost);

	m_attackedEnemyMap.clear();

	//auto& enemyManager = EnemyManager::GetEnemyManager();
	//enemyManager.ApplyAoeDamage(/*attack origin*/ player->GetPosition(), m_range, m_damageAmount * combo);
}

void PlayerAttackRevolvingTackle::Execute(Player* player) {

	if (!m_canDoAttack) {
		m_isDone = true;
		return;
	}

	auto delta = gameTime()->GetDeltaTime();
	//m_timer += delta;

	auto vel = player->GetForward();
	vel *= m_tacklePower;
	player->SetVelocity(vel);

	for (int i = 0; i < EnemyManager::GetEnemyManager().GetEnemies().size(); i++) {
		auto enemy = EnemyManager::GetEnemyManager().GetEnemies().at(i);
		auto& epos = enemy->GetPosition();
		if ((player->GetPosition() - epos).Length() < m_range) {
			if (m_attackedEnemyMap.find(enemy) == m_attackedEnemyMap.end()) {

				auto vecKb = EnemyManager::GetEnemyManager().GetEnemies().at(i)->GetPosition() - player->GetPosition();
				vecKb.y = 0;
				vecKb.Normalize();
				vecKb.y = 2.f;
				vecKb.Normalize();
				vecKb *= m_knockBackPower;

				EnemyManager::GetEnemyManager().GetEnemies().at(i)->ApplyDamage(m_damageAmount, true, vecKb);
				m_attackedEnemyMap.insert(std::make_pair(enemy, true));
			}
			//m_isDone = true;
		}
	}

	m_timer += gameTime()->GetDeltaTime();

	if (m_timer > m_REVOLVE_TIME)
		m_isDone = true;

}


