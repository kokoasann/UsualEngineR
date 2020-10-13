#include "stdafx.h"
#include "EnemySlashState.h"
#include "../IEnemy.h"
#include "../EnemyManager.h"
#include "../../Player/Player.h"

EnemySlashState::EnemySlashState() {
}

EnemySlashState::~EnemySlashState() {
}

void EnemySlashState::Enter(IEnemy* e) {
	printf("Enemy - Enter slash\n");
	m_timer = 0.f;
	//e->playAttackAnimation();
	auto player = EnemyManager::GetEnemyManager().GetPlayer();
	player->ApplyDamage(m_damageAmount);
}

IEnemyState* EnemySlashState::Update(IEnemy* e) {
	m_timer += gameTime()->GetDeltaTime();
	if (m_timer >= m_intervalSec) {
		return e->GetState(IEnemy::EnState::enBattleState);
	}
	return this;
}

void EnemySlashState::Exit(IEnemy* e) {
	printf("Enemy - Exit slash\n");
}