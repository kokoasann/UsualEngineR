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
	m_combo = combo;
	m_hasAlreadyAttacked = false;

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

}

void PlayerAttackPunch::Execute(Player* player) {

	if (!player->IsPlayingAnimation()) {
		m_timer += gameTime()->GetDeltaTime();
	}
	else{

		const float knockbackParam = 150.f;
		bool stunFlag = false;
		const int MAX_COMBO = 3;
		if (m_combo == MAX_COMBO) {
			stunFlag = true;
		}

		if (m_combo % 2 == 0) {
			if (player->ColCheck(Player::EnPlayerBone::enHand_L) and !m_hasAlreadyAttacked) {
				auto& enemyManager = EnemyManager::GetEnemyManager();
				auto nearestEnemy = enemyManager.GetNearestEnemy(player->GetBone(Player::EnPlayerBone::enHand_L)->GetWorldMatrix().GetTransrate());

				auto vecPlayerToEnemy = nearestEnemy->GetPosition() - player->GetPosition();
				vecPlayerToEnemy.Normalize();

				nearestEnemy->ApplyDamage(m_damageAmount * m_combo, stunFlag, vecPlayerToEnemy * knockbackParam);
				m_hasAlreadyAttacked = true;
			}
		}

		if (m_combo % 2 != 0) {
			if (player->ColCheck(Player::EnPlayerBone::enHand_R) and !m_hasAlreadyAttacked) {
				auto& enemyManager = EnemyManager::GetEnemyManager();
				auto nearestEnemy = enemyManager.GetNearestEnemy(player->GetBone(Player::EnPlayerBone::enHand_R)->GetWorldMatrix().GetTransrate());

				auto vecPlayerToEnemy = nearestEnemy->GetPosition() - player->GetPosition();
				vecPlayerToEnemy.Normalize();

				nearestEnemy->ApplyDamage(m_damageAmount * m_combo, stunFlag, vecPlayerToEnemy * knockbackParam);
				m_hasAlreadyAttacked = true;
			}
		}
	}

	if (m_timer >= m_interval) {
		m_isDone = true;
	}

	if (g_pad[0]->IsTrigger(enButtonB)) {
		m_isContinuAttack = true;
	}

}

