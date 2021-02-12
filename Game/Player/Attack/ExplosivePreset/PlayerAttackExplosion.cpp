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
	player->PlayAnimation(Player::EnAnimation::enTackle);

	player->UseStamina(m_StaminaCost);
	player->UseBoost(m_BoostCost);

	m_isBombed = false;

	//auto& enemyManager = EnemyManager::GetEnemyManager();
	//enemyManager.ApplyAoeDamage(/*attack origin*/ player->GetPosition(), m_range, m_damageAmount * combo);

}

void PlayerAttackExplosion::Execute(Player* player) {

	if (!m_canDoAttack) {
		m_isDone = true;
		return;
	}

	auto delta = gameTime()->GetDeltaTime();

	//m_timer += delta;

	auto vel = player->GetForward();
	vel *= m_tacklePower;

	if (m_isBombed)
		vel *= 0.f;

	player->SetVelocity(vel);

	/*
	for (int i = 0; i < EnemyManager::GetEnemyManager().GetEnemies().size(); i++) {
		auto& epos = EnemyManager::GetEnemyManager().GetEnemies().at(i)->GetPosition();
		if ((player->GetPosition() - epos).Length() < m_range) {
			m_isBombed = true;
			break;
		}
	}
	*/

	if (!m_isBombed and (player->GetPosition() - EnemyManager::GetEnemyManager().GetNearestEnemy(player->GetPosition())->GetPosition()).Length() < m_range){
		m_isBombed = true;
		for (int i = 0; i < EnemyManager::GetEnemyManager().GetEnemies().size(); i++) {
			auto& epos = EnemyManager::GetEnemyManager().GetEnemies().at(i)->GetPosition();
			if ((player->GetPosition() - epos).Length() < m_ExplodeDamageRange) {
				auto vecKb = EnemyManager::GetEnemyManager().GetEnemies().at(i)->GetPosition() - player->GetPosition();
				vecKb.y = 0;
				vecKb.Normalize();
				vecKb.y = 2.f;
				vecKb.Normalize();
				vecKb *= m_knockBackPower;
				EnemyManager::GetEnemyManager().GetEnemies().at(i)->ApplyDamage(m_explodeDamage,true, vecKb);
				auto se = NewGO<CSoundSource>(0);
				se->Init(L"Assets/sound/chara/explosion.wav", false);
				se->Play(false);
				player->Explode();
			}
		}
	}

	if (!player->IsPlayingAnimation()) {
		m_timer += gameTime()->GetDeltaTime();
	}

	if (m_timer >= m_interval) {
		m_isDone = true;
	}

}


