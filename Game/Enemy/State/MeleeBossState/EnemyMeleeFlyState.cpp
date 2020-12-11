#include "stdafx.h"
#include "EnemyMeleeFlyState.h"
#include "../../EnemyManager.h"
#include "../../IEnemy.h"
#include "../../../Player/Player.h"
#include "../../Boss/BossA.h"
#include "../../../Effect/JetEffect.h"

EnemyMeleeFlyState::EnemyMeleeFlyState() {

}

EnemyMeleeFlyState::~EnemyMeleeFlyState() {

}

void EnemyMeleeFlyState::Enter(IEnemy* e) {

#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Enter enemy fly\n");
#endif

	e->PlayAnimation(IEnemy::EnAnimation::enIdle);

	if (e->IsOnGround()) {
		m_flyTimer = 0.f;
	}

	if (e->GetAbility().stamina == e->GetAbility().max_stamina){
		m_flyTimer = 0.f;
	}

	auto& effects = e->GetJetEffects();
	effects.at(TO_INT(BossA::EnJetBone::ThrusterL))->SetGenerateFlag(true);
	effects.at(TO_INT(BossA::EnJetBone::ThrusterR))->SetGenerateFlag(true);

}

IEnemyState* EnemyMeleeFlyState::Update(IEnemy* e) {

	auto delta = gameTime()->GetDeltaTime();

	e->RecoverStamina(m_stamina_recover_amount * delta);

	auto player = GameManager::GetInstance().GetPlayer();

	if (player->GetCurrentHP() <= 0) {
		return e->GetState(TO_INT(IEnemy::EnState::enIdleState));
	}
	m_flyTimer += delta;

	//printf("time : %f\n", m_flyTimer);

	if (m_flyTimer > m_flyLimitTime) {
		return e->GetState(TO_INT(BossA::EnState::enOverheat));
	}

	auto& epos = e->GetPosition();
	auto& ppos = player->GetPosition();
	auto vecToPlayer = ppos - epos;
	auto len = vecToPlayer.Length();
	vecToPlayer.Normalize();
	vecToPlayer *= len * 0.8f;

	const float distLimit = 20.f;
	const float attackRange = 30.f;

	const float heightDiff = -4.f;
	//Fly
	if (len <= heightDiff) {
		//return e->GetState(TO_INT(IEnemy::EnState::enBattleState));
	}

	//Chase
	if (vecToPlayer.Length() > distLimit) {
		auto v = vecToPlayer;
		v.Normalize();
		auto velocity = v * m_chaseSpeed;
		e->SetVelocity(velocity);
	}

	//Teleportation
	const float teleportationDist = 80.f;
	if (vecToPlayer.Length() > teleportationDist) {
		return e->GetState(TO_INT(BossA::EnState::enTeleportation));
	}

	if (vecToPlayer.Length() < attackRange) {
		auto rnd = GRandom().Rand();
		if (rnd >= 0.5f) {
			return e->GetState(TO_INT(BossA::EnState::enPunch));
		}
		else {
			return e->GetState(TO_INT(BossA::EnState::enDashPunch));
		}
	}

	return this;
}

void EnemyMeleeFlyState::Exit(IEnemy* e) {
#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Exit enemy fly\n");
#endif
}