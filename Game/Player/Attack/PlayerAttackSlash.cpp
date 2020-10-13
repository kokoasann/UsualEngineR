#include "stdafx.h"
#include "PlayerAttackSlash.h"
#include "../Player.h"
#include "../../Enemy/EnemyManager.h"

PlayerAttackSlash::PlayerAttackSlash() {

}

PlayerAttackSlash::~PlayerAttackSlash() {

}

void PlayerAttackSlash::Init(Player* player, int combo) {
	printf("Slash combo : %d\n", combo);
	m_isDone = false;
	m_isContinuAttack = false;
	m_timer = 0.f;
	player->PlayAnimation(Player::EnAnimation::enAttack);

	auto& enemyManager = EnemyManager::GetEnemyManager();
	enemyManager.ApplyAoeDamage(/*attack origin*/ player->GetPosition(), m_range,  m_damageAmount * combo);
}

void PlayerAttackSlash::Execute(Player* player) {
	//TODO : if(!animation.isPlay()) m_timer += deltaTime(); 
	m_timer += gameTime()->GetDeltaTime();

	if (g_pad[0]->IsTrigger(enButtonB)) {
		m_isContinuAttack = true;
	}
	if (m_timer >= m_interval) {
		m_isDone = true;
	}
}
