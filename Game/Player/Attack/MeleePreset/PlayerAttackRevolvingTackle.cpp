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
	m_isDone = false;
	m_isContinuAttack = false;
	m_timer = 0.f;
	player->PlayAnimation(Player::EnAnimation::enAttack);

	//auto& enemyManager = EnemyManager::GetEnemyManager();
	//enemyManager.ApplyAoeDamage(/*attack origin*/ player->GetPosition(), m_range, m_damageAmount * combo);
}

void PlayerAttackRevolvingTackle::Execute(Player* player) {
	auto delta = gameTime()->GetDeltaTime();
	m_timer += delta;

	auto vel = player->GetForward();
	vel *= m_tacklePower;
	player->SetVelocity(vel);

	for (int i = 0; i < EnemyManager::GetEnemyManager().GetEnemies().size(); i++) {
		auto& epos = EnemyManager::GetEnemyManager().GetEnemies().at(i)->GetPosition();
		if ((player->GetPosition() - epos).Length() < m_range) {
			EnemyManager::GetEnemyManager().GetEnemies().at(i)->ApplyDamage(m_damageAmount);
			m_isDone = true;
		}
	}

	if (m_timer > m_interval)
		m_isDone = true;

}


