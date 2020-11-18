#include "stdafx.h"
#include "EnemyMeleeFlyState.h"
#include "../../EnemyManager.h"
#include "../../IEnemy.h"
#include "../../../Player/Player.h"
#include "../../Boss/BossA.h"

EnemyMeleeFlyState::EnemyMeleeFlyState() {

}

EnemyMeleeFlyState::~EnemyMeleeFlyState() {

}

void EnemyMeleeFlyState::Enter(IEnemy* e) {

	e->PlayAnimation(IEnemy::EnAnimation::enIdle);

	m_flyTimer = 0.f;

#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Enter enemy fly\n");
#endif
}

IEnemyState* EnemyMeleeFlyState::Update(IEnemy* e) {

	auto player = EnemyManager::GetEnemyManager().GetPlayer();

	if (player->GetCurrentHP() <= 0) {
		return e->GetState(TO_INT(BossA::EnState::enIdleState));
	}


	auto& epos = e->GetPosition();
	auto& ppos = player->GetPosition();
	auto vecToPlayer = ppos - epos;
	auto len = vecToPlayer.Length();
	vecToPlayer.Normalize();
	vecToPlayer *= len * 0.8f;

	const float distLimit = 20.f;
	const float attackRange = 30.f;

	//Chase
	if (vecToPlayer.Length() > distLimit) {
		vecToPlayer.Normalize();
		const float speed = 200.f;
		auto velocity = vecToPlayer * speed;
		e->SetVelocity(velocity);
	}


	if (vecToPlayer.Length() < attackRange) {
		auto rnd = GRandom().Rand();
		if (rnd >= 0.5f) {
			return e->GetState(IEnemy::EnState::enAttackA);
		}
		else {
			return e->GetState(IEnemy::EnState::enAttackB);
		}
	}

	auto delta = gameTime()->GetDeltaTime();
	m_flyTimer + delta;

	if(m_flyTimer > m_flyLimitTime)
		return e->GetStateList().at(TO_INT(IEnemy::EnState::enBattleState));

}

void EnemyMeleeFlyState::Exit(IEnemy* e) {
#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Exit enemy fly\n");
#endif
}