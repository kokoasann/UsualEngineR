#include "stdafx.h"
#include "EnemyMeleeBattleState.h"
#include "../../EnemyManager.h"
#include "../../IEnemy.h"
#include "../../../Player/Player.h"
#include "../../Boss/BossA.h"
#include "../../../Effect/JetEffect.h"
#include "../../GameManager.h"
#include "../../../Game.h"

EnemyMeleeBattleState::EnemyMeleeBattleState() {

}

EnemyMeleeBattleState::~EnemyMeleeBattleState() {

}

void EnemyMeleeBattleState::Enter(IEnemy* e) {

	e->PlayAnimation(IEnemy::EnAnimation::enIdle);

	auto& effects = e->GetJetEffects();
	effects.at(TO_INT(BossA::EnJetBone::ThrusterL))->SetGenerateFlag(false);
	effects.at(TO_INT(BossA::EnJetBone::ThrusterR))->SetGenerateFlag(false);

	if (!m_isPerformed) {
		GameManager::GetInstance().m_gameScene->OnEnterBattle(e);
		m_isPerformed = true;
	}
#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Enter enemy melee battle\n");
#endif
}

IEnemyState* EnemyMeleeBattleState::Update(IEnemy* e) {

	auto delta = gameTime()->GetDeltaTime();

	e->RecoverStamina(m_stamina_recover_amount * delta);

	const auto stamina = e->GetAbility().stamina;

	auto player = EnemyManager::GetEnemyManager().GetPlayer();

	if (player->GetCurrentHP() <= 0) {
		return e->GetState(TO_INT(IEnemy::EnState::enIdleState));
	}

	if (player->GetCurrentState() == player->GetState(Player::EnState::enFlying)){
		return e->GetState(TO_INT(BossA::EnState::enFlyState));
	}

	auto& epos = e->GetPosition();
	auto& ppos = player->GetPosition();
	auto vecToPlayer = ppos - epos;
	const float distLimit = 20.f;
	const float attackRange = 5.f;
	const float teleportationDist = 200.f;

	//Teleportation
	if (vecToPlayer.Length() > teleportationDist) {
		return e->GetState(TO_INT(BossA::EnState::enTeleportation));
	}

	const float GRAVITY = -30.f;

	//Chase
	if (vecToPlayer.Length() > distLimit) {
		auto v = vecToPlayer;
		v.Normalize();
		const float speed = 50.f;
		auto velocity = v * speed;

		if(!e->IsOnGround())
			velocity.y = GRAVITY;

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

	//ó‘Ô‚ª•Ï‚í‚ç‚È‚¢‚È‚ç©•ª‚ğ•Ô‚·
	return this;
}

void EnemyMeleeBattleState::Exit(IEnemy* e) {
#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Exit enemy melee battle\n");
#endif
}