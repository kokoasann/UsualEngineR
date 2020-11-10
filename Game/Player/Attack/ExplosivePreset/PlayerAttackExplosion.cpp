#include "stdafx.h"
#include "PlayerAttackExplosion.h"
#include "../../Player.h"
#include "../../../Enemy/EnemyManager.h"

PlayerAttackExplosion::PlayerAttackExplosion() {

}

PlayerAttackExplosion::~PlayerAttackExplosion() {

}

void PlayerAttackExplosion::Init(Player* player, int combo) {
#ifdef _PRINT_PLAYER_ATTACK
	std::string s = "attack explosion combo :" + std::to_string(combo);
	DebugPrint_WATA(s.c_str());
#endif
	m_isDone = false;
	m_isContinuAttack = false;
	m_timer = 0.f;
	player->PlayAnimation(Player::EnAnimation::enAttack);

	m_isBombed = false;

	//auto& enemyManager = EnemyManager::GetEnemyManager();
	//enemyManager.ApplyAoeDamage(/*attack origin*/ player->GetPosition(), m_range, m_damageAmount * combo);

}

void PlayerAttackExplosion::Execute(Player* player) {
	auto delta = gameTime()->GetDeltaTime();
	m_timer += delta;

	auto vel = player->GetForward();
	vel *= m_tacklePower;

	if (m_isBombed)
		vel *= 0.f;

	player->SetVelocity(vel);

	for (int i = 0; i < EnemyManager::GetEnemyManager().GetEnemies().size(); i++) {
		auto& epos = EnemyManager::GetEnemyManager().GetEnemies().at(i)->GetPosition();
		if ((player->GetPosition() - epos).Length() < m_range) {
			m_isBombed = true;
			break;
		}
	}

	if (m_isBombed) {
		for (int i = 0; i < EnemyManager::GetEnemyManager().GetEnemies().size(); i++) {
			auto& epos = EnemyManager::GetEnemyManager().GetEnemies().at(i)->GetPosition();
			if ((player->GetPosition() - epos).Length() < m_ExplodeDamageRange) {
				EnemyManager::GetEnemyManager().GetEnemies().at(i)->ApplyDamage(m_explodeDamage);
			}
		}
	}

	if (m_timer > m_interval)
		m_isDone = true;
}


