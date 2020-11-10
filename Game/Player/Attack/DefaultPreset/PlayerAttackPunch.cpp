#include "stdafx.h"
#include "PlayerAttackPunch.h"
#include "../../Player.h"
#include "../../../Enemy/EnemyManager.h"

PlayerAttackPunch::PlayerAttackPunch() {

}

PlayerAttackPunch::~PlayerAttackPunch() {

}

void PlayerAttackPunch::Init(Player* player, int combo) {
#ifdef _PRINT_PLAYER_ATTACK
	std::string s = "attack punch combo :" + std::to_string(combo);
	DebugPrint_WATA(s.c_str());
#endif
	m_isDone = false;
	m_isContinuAttack = false;
	m_timer = 0.f;

	switch (combo) {
	case 1:
		player->PlayAnimation(Player::EnAnimation::enPunch1);
		break;
	case 2:
		player->PlayAnimation(Player::EnAnimation::enPunch2);
		break;
	case 3:
		player->PlayAnimation(Player::EnAnimation::enPunch3);
		break;
	}

	auto& enemyManager = EnemyManager::GetEnemyManager();
	enemyManager.ApplyAoeDamage(/*attack origin*/ player->GetPosition(), m_range, m_damageAmount * combo);
}

void PlayerAttackPunch::Execute(Player* player) {
	//TODO : if(!animation.isPlay()) m_timer += deltaTime(); 
	m_timer += gameTime()->GetDeltaTime();

	if (g_pad[0]->IsTrigger(enButtonB)) {
		m_isContinuAttack = true;
	}
	if (m_timer >= m_interval) {
		m_isDone = true;
	}
}

