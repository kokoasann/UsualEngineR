#include "stdafx.h"
#include "PlayerAttackThrowBomb.h"
#include "../../Player.h"
#include "../../../Enemy/EnemyManager.h"
#include "../Bomb.h"

PlayerAttackThrowBomb::PlayerAttackThrowBomb() {

}

PlayerAttackThrowBomb::~PlayerAttackThrowBomb() {

}

void PlayerAttackThrowBomb::Init(Player* player, int combo) {
#ifdef _PRINT_PLAYER_ATTACK
	std::string s = "attack throw bomb combo :" + std::to_string(combo);
	DebugPrint_WATA(s.c_str());
#endif
	m_isDone = false;
	m_isContinuAttack = false;
	m_timer = 0.f;
	player->PlayAnimation(Player::EnAnimation::enAttack);

	//auto& enemyManager = EnemyManager::GetEnemyManager();
	//enemyManager.ApplyAoeDamage(/*attack origin*/ player->GetPosition(), m_range, m_damageAmount * combo);

	auto bomb = NewGO<Bomb>(0);
	auto vel = player->GetForward() * m_speed;
	bomb->Init(player->GetPosition(), vel);

}

void PlayerAttackThrowBomb::Execute(Player* player) {
	//TODO : if(!animation.isPlay()) m_timer += deltaTime(); 
	//m_timer += gameTime()->GetDeltaTime();

	if (g_pad[0]->IsTrigger(enButtonB)) {
		m_isContinuAttack = true;
	}

	if (!player->IsPlayingAnimation()) {
		m_timer += gameTime()->GetDeltaTime();
	}

	if (m_timer >= m_interval) {
		m_isDone = true;
	}
}


