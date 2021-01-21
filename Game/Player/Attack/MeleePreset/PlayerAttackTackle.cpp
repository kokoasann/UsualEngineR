#include "stdafx.h"
#include "PlayerAttackTackle.h"
#include "../../Player.h"
#include "../../../Enemy/EnemyManager.h"

PlayerAttackTackle::PlayerAttackTackle() {

}

PlayerAttackTackle::~PlayerAttackTackle() {

}

void PlayerAttackTackle::Init(Player* player, int combo) {
#ifdef _PRINT_PLAYER_ATTACK
	std::string s = "attack tackle combo :" + std::to_string(combo);
	DebugPrint_WATA(s.c_str());
#endif
	m_isDone = false;
	m_isContinuAttack = false;
	m_timer = 0.f;
	player->PlayAnimation(Player::EnAnimation::enTackle);

	//auto& enemyManager = EnemyManager::GetEnemyManager();
	//enemyManager.ApplyAoeDamage(/*attack origin*/ player->GetPosition(), m_range, m_damageAmount * combo);
	player->FireThrusters();

}

void PlayerAttackTackle::Execute(Player* player) {
	auto delta = gameTime()->GetDeltaTime();
	//m_timer += delta;

	auto vel = player->GetForward();
	vel *= m_tacklePower;
	player->SetVelocity(vel);

	for (int i = 0; i < EnemyManager::GetEnemyManager().GetEnemies().size(); i++) {
		auto& epos = EnemyManager::GetEnemyManager().GetEnemies().at(i)->GetPosition();
		if ((player->GetPosition() - epos).Length() < m_range) {

			auto vecKb = EnemyManager::GetEnemyManager().GetEnemies().at(i)->GetPosition() - player->GetPosition();
			vecKb.y = 0;
			vecKb.Normalize();
			vecKb.y = 2.f;
			vecKb.Normalize();
			vecKb *= m_knockBackPower;

			EnemyManager::GetEnemyManager().GetEnemies().at(i)->ApplyDamage(m_damageAmount, true, vecKb);

			auto se = NewGO< CSoundSource>(0);
			se->Init(L"Assets/sound/chara/punch1.wav", true);
			se->Play(false);

			m_isDone = true;
		}
	}

	if (!player->IsPlayingAnimation()) {
		m_timer += gameTime()->GetDeltaTime();
	}

	if(m_timer > m_interval)
		m_isDone = true;

}


