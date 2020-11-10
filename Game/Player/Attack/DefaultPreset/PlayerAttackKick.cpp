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
	m_isDone = false;
	m_timer = 0.f;
	player->PlayAnimation(Player::EnAnimation::enAttack);

	auto& enemyManager = EnemyManager::GetEnemyManager();
	enemyManager.ApplyAoeDamage(/*attack origin*/ player->GetPosition(), m_range, m_damageAmount * combo);
}

void PlayerAttackKick::Execute(Player* player) {
	//TODO : if(!animation.isPlay()) m_timer += deltaTime(); 
	m_timer += gameTime()->GetDeltaTime();

	if (m_timer >= m_interval) {
		m_isDone = true;
	}
}
