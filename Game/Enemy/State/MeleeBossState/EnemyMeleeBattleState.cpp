#include "stdafx.h"
#include "EnemyMeleeBattleState.h"
#include "../../EnemyManager.h"
#include "../../IEnemy.h"
#include "../../../Player/Player.h"
#include "../../Boss/BossA.h"
#include "../../../Effect/JetEffect.h"
#include "GameManager.h"
#include "../../../Game.h"

EnemyMeleeBattleState::EnemyMeleeBattleState() {

}

EnemyMeleeBattleState::~EnemyMeleeBattleState() {

}

void EnemyMeleeBattleState::Enter(IEnemy* e) {

#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Enter enemy melee battle\n");
#endif

	e->PlayAnimation(IEnemy::EnAnimation::enIdle);

	auto& effects = e->GetJetEffects();
	effects.at(TO_INT(BossA::EnJetBone::ThrusterL))->SetGenerateFlag(false);
	effects.at(TO_INT(BossA::EnJetBone::ThrusterR))->SetGenerateFlag(false);

	auto hp = e->GetCurrentHP();

	// :---phase3--:---phase2---:---phase1---://
	const auto div = e->GetMaxHP() / TO_INT(EnBattlePhase::NumPhase);
	const float minPhase1 = e->GetMaxHP() - div;
	const float minPhase2 = e->GetMaxHP() - div * 2;

	if (hp < minPhase2) {
		//phase3
		m_battlePhase = EnBattlePhase::Normal;
		//printf("phase 3\n");
	}

	if (hp >= minPhase2 and hp < minPhase1) {
		//phase2
		m_battlePhase = EnBattlePhase::Mad;
		//printf("phase 2\n");
	}

	if (hp >= minPhase1) {
		//phase 1
		m_battlePhase = EnBattlePhase::Calm;
		//printf("phase 1\n");
	}


	if (!m_isPerformed) {
		GameManager::GetInstance().m_gameScene->OnEnterBattle(e);
		m_isPerformed = true;
	}

}

IEnemyState* EnemyMeleeBattleState::Update(IEnemy* e) {

	auto delta = gameTime()->GetDeltaTime();

	const float staminaRecoverSpeedMadParam = 500.f;
	const float staminaRecoverSpeedNormalParam = 1.5f;

	switch (m_battlePhase) {
	case EnBattlePhase::Calm:
		e->RecoverStamina(m_stamina_recover_amount * delta);
		break;
	case EnBattlePhase::Normal:
		e->RecoverStamina(m_stamina_recover_amount * staminaRecoverSpeedNormalParam * delta);
		break;
	case EnBattlePhase::Mad:
		e->RecoverStamina(m_stamina_recover_amount * staminaRecoverSpeedMadParam * delta);
		break;
	}

	const auto stamina = e->GetAbility().stamina;

	auto player = GameManager::GetInstance().GetPlayer();

	if (player->GetCurrentHP() <= 0) {
		return e->GetState(TO_INT(IEnemy::EnState::enIdleState));
	}

	auto& epos = e->GetPosition();
	auto& ppos = player->GetPosition();
	auto vecToPlayer = ppos - epos;
	const float distLimit = 20.f;
	const float PunchAttackRange = e->GetCharaRadius() + player->GetCharaRadius() + 5.f;
	const float ThrusterAttackRange = e->GetCharaRadius() + player->GetCharaRadius() + 25.f;

	const float flyDist = 0.f;
	const auto rnd = GRandom().Rand();


	const float GRAVITY = -30.f;

	//Chase
	if (vecToPlayer.Length() > distLimit) {
		auto v = vecToPlayer;
		v.Normalize();
		const float speed = 50.f;
		auto velocity = v * speed;

		if (!e->IsOnGround())
			velocity.y = GRAVITY;

		e->SetVelocity(velocity);
	}

	//Teleportation
	m_tpTimer += delta;

	if (m_battlePhase == EnBattlePhase::Calm) {
		const float teleportationDist = 200.f;
		const float tpIntervalSec = 3.f;
		const float tpChance = 0.1; // 10% chance

		if ((vecToPlayer.Length() > teleportationDist) or (m_tpTimer > tpIntervalSec and vecToPlayer.Length() > PunchAttackRange and rnd <= tpChance)) {
			return e->GetState(TO_INT(BossA::EnState::enTeleportation));
			m_tpTimer = 0.f;
		}
	}
	else if (m_battlePhase == EnBattlePhase::Normal) {
		const float teleportationDist = 120.f;
		const float tpIntervalSec = 2.f;
		const float tpChance = 0.5; // 50% chance

		if ((vecToPlayer.Length() > teleportationDist) or (m_tpTimer > tpIntervalSec and vecToPlayer.Length() > PunchAttackRange and rnd <= tpChance)) {
			return e->GetState(TO_INT(BossA::EnState::enTeleportation));
			m_tpTimer = 0.f;
		}
	}
	else if (m_battlePhase == EnBattlePhase::Mad) {
		const float teleportationDist = 70.f;
		const float tpIntervalSec = 1.5f;
		const float tpChance = 0.7; // 70% chance

		if ((vecToPlayer.Length() > teleportationDist) or (m_tpTimer > tpIntervalSec and vecToPlayer.Length() > PunchAttackRange and rnd <= tpChance)) {
			return e->GetState(TO_INT(BossA::EnState::enTeleportation));
			m_tpTimer = 0.f;
		}
	}

	//Fly
	if (player->GetCurrentState() == player->GetState(Player::EnState::enFlying) or vecToPlayer.y > flyDist) {
		return e->GetState(TO_INT(BossA::EnState::enFlyState));
	}

	//printf("TP TIMER : %f\n",m_tpTimer);

	/*
	if (m_tpTimer > tpIntervalSec) {
		m_tpTimer = 0.f;
	}
	*/

	//}

	if (vecToPlayer.Length() < ThrusterAttackRange) {
		const float thrustTime = 0.3f;
		m_thrustTimer += delta;
		if (m_thrustTimer >= thrustTime) {
			m_thrustTimer = 0.f;
			return e->GetState(TO_INT(BossA::EnState::enThrusterAttack));
		}
	}

	if (vecToPlayer.Length() < PunchAttackRange) {
		const float numAttack = 2.f;
		const float chance = 1.f / numAttack;
		const float c1 = chance;
		const float c2 = chance * 2.f;
		//const float c3 = chance *3.f;

		if (rnd >= 0 and rnd <= c1) {
			//attack1
			return e->GetState(TO_INT(BossA::EnState::enPunch));
		}
		else if (rnd > c1 and rnd <= c2) {
			//attack2
			return e->GetState(TO_INT(BossA::EnState::enDashPunch));
		}
	}

	//状態が変わらないなら自分を返す
	return this;
}

void EnemyMeleeBattleState::Exit(IEnemy* e) {
#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Exit enemy melee battle\n");
#endif
}