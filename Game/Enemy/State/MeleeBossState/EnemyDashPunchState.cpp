#include "stdafx.h"
#include "EnemyDashPunchState.h"
#include "../../EnemyManager.h"
#include "../../IEnemy.h"
#include "../../../Player/Player.h"
#include "../../Boss/BossA.h"
#include "../../../Effect/JetEffect.h"

EnemyDashPunchState::EnemyDashPunchState() {

}

EnemyDashPunchState::~EnemyDashPunchState() {

}

void EnemyDashPunchState::Enter(IEnemy* e) {

	m_canExecute = true;

	if (e->GetAbility().stamina < m_cost) {
		m_canExecute = false;
		return;
	}


	e->UseStamina(m_cost);

	e->PlayAnimation(IEnemy::EnAnimation::enAttackB);
	m_timer = 0.f;
	//e->playAttackAnimation();
	auto player = GameManager::GetInstance().m_player;

	auto& epos = e->GetPosition();
	auto& ppos = player->GetPosition();
	auto vecToPlayer = ppos - epos;
	auto v = vecToPlayer;
	v.Normalize();
	const float speed = 100.f;
	auto velocity = v * speed;
	e->SetVelocity(velocity);

	auto& effects = e->GetJetEffects();
	effects.at(TO_INT(BossA::EnJetBone::ThrusterL))->SetGenerateFlag(true);
	effects.at(TO_INT(BossA::EnJetBone::ThrusterR))->SetGenerateFlag(true);

	effects.at(TO_INT(BossA::EnJetBone::ElbowL))->SetGenerateFlag(true);
	effects.at(TO_INT(BossA::EnJetBone::ElbowR))->SetGenerateFlag(true);

	const float knockbackParam = 250.f;
	v *= knockbackParam;
	v.y = 150.f;
	player->ApplyDamage(m_damageAmount, true, v);
	auto punchSE = NewGO<CSoundSource>(0);
	punchSE->Init(L"Assets/sound/chara/punch_2_1.wav", false);
	punchSE->Play(false);

#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Enter enemy dash punch state\n");
#endif
}

IEnemyState* EnemyDashPunchState::Update(IEnemy* e) {

	if (!m_canExecute) {
		return e->GetState(TO_INT(IEnemy::EnState::enBattleState));
	}

	auto player = GameManager::GetInstance().GetPlayer();

	auto& epos = e->GetPosition();
	auto& ppos = player->GetPosition();
	auto vecToPlayer = ppos - epos;

	m_timer += gameTime()->GetDeltaTime();
	if (m_timer >= m_intervalSec) {
		return e->GetState(TO_INT(IEnemy::EnState::enBattleState));
	}
	return this;
}

void EnemyDashPunchState::Exit(IEnemy* e) {

	auto& effects = e->GetJetEffects();
	effects.at(TO_INT(BossA::EnJetBone::ThrusterL))->SetGenerateFlag(false);
	effects.at(TO_INT(BossA::EnJetBone::ThrusterR))->SetGenerateFlag(false);

	effects.at(TO_INT(BossA::EnJetBone::ElbowL))->SetGenerateFlag(false);
	effects.at(TO_INT(BossA::EnJetBone::ElbowR))->SetGenerateFlag(false);

#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Exit enemy dush punch state\n");
#endif
}
