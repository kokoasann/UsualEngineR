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

	e->PlayAnimation(IEnemy::EnAnimation::enIdle);

	bool flag = e->IsOnGround();
	if (flag) {
		m_flyTimer = 0.f;
	}

	auto& effects = e->GetJetEffects();
	effects.at(TO_INT(BossA::EnJetBone::ThrusterL))->SetGenerateFlag(true);
	effects.at(TO_INT(BossA::EnJetBone::ThrusterR))->SetGenerateFlag(true);

#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Enter enemy fly\n");
#endif
}

IEnemyState* EnemyMeleeFlyState::Update(IEnemy* e) {

	auto delta = gameTime()->GetDeltaTime();

	e->RecoverStamina(m_stamina_recover_amount * delta);

	auto player = EnemyManager::GetEnemyManager().GetPlayer();

	if (player->GetCurrentHP() <= 0) {
		return e->GetState(TO_INT(IEnemy::EnState::enIdleState));
	}
	m_flyTimer += delta;

	printf("time : %f\n", m_flyTimer);

	if (m_flyTimer > m_flyLimitTime) {
		return e->GetState(TO_INT(BossA::EnState::enOverheat));
	}

	if (player->GetCurrentState() != player->GetState(Player::EnState::enFlying)) {
		return e->GetState(TO_INT(IEnemy::EnState::enBattleState));
	}

	auto& epos = e->GetPosition();
	auto& ppos = player->GetPosition();
	auto vecToPlayer = ppos - epos;
	auto len = vecToPlayer.Length();
	vecToPlayer.Normalize();
	vecToPlayer *= len * 0.8f;

	const float distLimit = 20.f;
	const float attackRange = 30.f;

	//Teleportation
	const float teleportationDist = 80.f;
	if (vecToPlayer.Length() > teleportationDist) {
		return e->GetState(TO_INT(BossA::EnState::enTeleportation));
	}

	//Chase
	if (vecToPlayer.Length() > distLimit) {
		auto v = vecToPlayer;
		v.Normalize();
		const float speed = 200.f;
		auto velocity = v * speed;
		e->SetVelocity(velocity);
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