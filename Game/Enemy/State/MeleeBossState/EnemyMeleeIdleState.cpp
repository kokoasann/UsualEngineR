#include "stdafx.h"
#include "EnemyMeleeIdleState.h"
#include "../../EnemyManager.h"
#include "GameManager.h"
#include "Game.h"
#include "../../IEnemy.h"
#include "../../../Player/Player.h"
#include "../../Boss/BossA.h"
#include "Enemy/BossBGM.h"

EnemyMeleeIdleState::EnemyMeleeIdleState() {

}

EnemyMeleeIdleState::~EnemyMeleeIdleState() {

}

void EnemyMeleeIdleState::Enter(IEnemy* e) {

	g_graphicsEngine->GetPostEffect().SetUseFocusBlurFrag(false);

	e->PlayAnimation(IEnemy::EnAnimation::enIdle);

	e->SetVelocity(Vector3::Zero);

	m_isAttacked = false;

	if (GameManager::GetInstance().m_bgm != nullptr) {
		GameManager::GetInstance().m_bgm->AddBgmCount(-1);
	}
#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Enter enemy melee idle\n");
#endif
}

IEnemyState* EnemyMeleeIdleState::Update(IEnemy* e) {

	auto player = GameManager::GetInstance().GetPlayer();
	if (player == nullptr) return this;

	auto& epos = e->GetPosition();
	auto& ppos = player->GetPosition();
	auto vecToPlayer = ppos - epos;
	const float chaseRange = 60.f;

	const float GRAVITY = -50.f;
	auto vel = Vector3::Zero;
	vel.y = GRAVITY;
	//if(!e->IsOnGround())
//		e->SetVelocity(vel);

	if ((vecToPlayer.Length() < chaseRange and player->GetCurrentHP() > 0.f) or m_isAttacked) {
		if (GameManager::GetInstance().m_bgm != nullptr) {
			GameManager::GetInstance().m_bgm->AddBgmCount(1);
		}
		return e->GetStateMap().at(TO_INT(IEnemy::EnState::enBattleState));
	}

	return this;
}

void EnemyMeleeIdleState::Exit(IEnemy* e) {
#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Exit enemy melee idle\n");
#endif
}

void EnemyMeleeIdleState::OnAttacked(IEnemy* e) {
	//printf("boss is attacked by player\n");
	m_isAttacked = true;
}
