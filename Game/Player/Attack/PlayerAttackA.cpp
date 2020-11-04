#include "stdafx.h"
#include "PlayerAttackA.h"
#include "../Player.h"
#include "../../Enemy/EnemyManager.h"

PlayerAttackA::PlayerAttackA() {

}

PlayerAttackA::~PlayerAttackA() {

}

void PlayerAttackA::Init(Player* player, int combo) {
	printf("attack A combo : %d\n", combo);
	m_isDone = false;
	m_isContinuAttack = false;
	m_timer = 0.f;
	player->PlayAnimation(Player::EnAnimation::enAttack);

	auto& enemyManager = EnemyManager::GetEnemyManager();
	enemyManager.ApplyAoeDamage(/*attack origin*/ player->GetPosition(), m_range, m_damageAmount * combo);
}

void PlayerAttackA::Execute(Player* player) {
	//TODO : if(!animation.isPlay()) m_timer += deltaTime(); 
	m_timer += gameTime()->GetDeltaTime();

	if (g_pad[0]->IsTrigger(enButtonB)) {
		m_isContinuAttack = true;
	}
	if (m_timer >= m_interval) {
		m_isDone = true;
	}
}

