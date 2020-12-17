#include "stdafx.h"
#include <cmath>
#include "EnemyIdleState.h"
#include "../EnemyManager.h"
#include "../IEnemy.h"
#include "../../Player/Player.h"
#include "GameManager.h"

EnemyIdleState::EnemyIdleState() {

}

EnemyIdleState::~EnemyIdleState() {

}

void EnemyIdleState::Enter(IEnemy* e) {

	e->PlayAnimation(IEnemy::EnAnimation::enIdle);

#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Enter enemy Idle\n");
#endif
}

IEnemyState* EnemyIdleState::Update(IEnemy* e) {

	//“K“–‚É“®‚©‚µ‚Ä‚İ‚½‚¾‚¯
	static float time = 0.f;
	time += gameTime()->GetDeltaTime();
	auto add = std::sin(time);
	Vector3 vel = Vector3::Zero;
	vel.x = add;
	vel.y -= 10.f;
	//e->SetVelocity(vel);

	e->SetVelocity({ 0,-30,0 });

	auto player = GameManager::GetInstance().m_player;
	auto& epos = e->GetPosition();
	auto& ppos = player->GetPosition();
	auto vecToPlayer = ppos - epos;
	const float chaseRange = 300.f;

	if (vecToPlayer.Length() < chaseRange and player->GetCurrentHP() > 0.f) {
		return e->GetState(TO_INT(IEnemy::EnState::enBattleState));
	}

	//ó‘Ô‚ª•Ï‚í‚ç‚È‚¢‚È‚ç©•ª‚ğ•Ô‚·
	return this;
}

void EnemyIdleState::Exit(IEnemy* e) {
#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Exit enemy Idle\n");
#endif
}