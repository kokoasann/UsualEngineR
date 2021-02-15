#include "stdafx.h"
#include "PlayerAttackRevolvingTackle.h"
#include "../../Player.h"
#include "../../../Enemy/EnemyManager.h"
#include "Camera/GameCamera.h"

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


	m_tackleVelocity = player->GetForward();

	auto gc = GameManager::GetInstance().GetGameCamera();
	if (gc != nullptr and gc->IsTargettingEnemy()) {
		auto target = EnemyManager::GetEnemyManager().GetTargettingEnemy();
		m_tackleVelocity = target->GetPosition() - player->GetPosition();
		m_tackleVelocity.Normalize();

		auto& crot = g_camera3D->GetCameraRotation();
		player->SetRotation(crot);
	}

	m_tackleVelocity *= m_tacklePower;
	player->SetVelocity(m_tackleVelocity);

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

				auto se = NewGO< CSoundSource>(0);
				se->Init(L"Assets/sound/chara/punch2.wav", true);
				se->Play(false);

				m_attackedEnemyMap.insert(std::make_pair(enemy, true));
			}
			//m_isDone = true;
		}
	}

	m_timer += gameTime()->GetDeltaTime();

	if (m_timer > m_REVOLVE_TIME)
		m_isDone = true;

}


