#include "stdafx.h"
#include "EnemyMeleeThrusterAttackState.h"
#include "../../EnemyManager.h"
#include "../../IEnemy.h"
#include "../../../Player/Player.h"
#include "../../Boss/BossA.h"
#include "GameManager.h"
#include "Effect/JetEffect.h"

EnemyMeleeThrusterAttackState::EnemyMeleeThrusterAttackState() {

}

EnemyMeleeThrusterAttackState::~EnemyMeleeThrusterAttackState() {

}

void EnemyMeleeThrusterAttackState::Enter(IEnemy* e) {

#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Enter melee thruster attack\n");
#endif

	m_canExecute = true;

	if (e->GetAbility().stamina < m_cost) {
		m_canExecute = false;
		return;
	}

	e->PlayAnimation(IEnemy::EnAnimation::enIdle);

	auto& effects = e->GetJetEffects();
	effects.at(TO_INT(BossA::EnJetBone::ThrusterL))->SetScale(0.5f);
	effects.at(TO_INT(BossA::EnJetBone::ThrusterR))->SetScale(0.5f);
	effects.at(TO_INT(BossA::EnJetBone::Skirt))->SetScale(0.5f);

	m_fireTimer = 0.f;

	e->UseStamina(m_cost);
}

IEnemyState* EnemyMeleeThrusterAttackState::Update(IEnemy* e) {

	if (!m_canExecute) {
		return e->GetState(TO_INT(IEnemy::EnState::enBattleState));
	}

	auto player = GameManager::GetInstance().GetPlayer();

	auto dist = (player->GetPosition() - e->GetPosition()).Length();

	if (dist < m_range) {
		const float knockBackParam = 150.f;
		auto vecToPlayer = player->GetPosition() - e->GetPosition();
		vecToPlayer.Normalize();
		vecToPlayer *= knockBackParam;
		player->ApplyDamage(m_damage, true, vecToPlayer);
	}

	if (m_fireTimer > m_fireTime) {
		return e->GetState(TO_INT(IEnemy::EnState::enBattleState));
	}

	return this;
}

void EnemyMeleeThrusterAttackState::Exit(IEnemy* e) {
#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Exit melee thruster attack\n");
#endif

	auto& effects = e->GetJetEffects();
	effects.at(TO_INT(BossA::EnJetBone::ThrusterL))->SetScale(0.1f);
	effects.at(TO_INT(BossA::EnJetBone::ThrusterR))->SetScale(0.1f);
	effects.at(TO_INT(BossA::EnJetBone::Skirt))->SetScale(0.1f);

}