#include "stdafx.h"
#include "EnemyMeleeIdleState.h"
#include "../../EnemyManager.h"
#include "../../IEnemy.h"
#include "../../../Player/Player.h"

EnemyMeleeIdleState::EnemyMeleeIdleState() {

}

EnemyMeleeIdleState::~EnemyMeleeIdleState() {

}

void EnemyMeleeIdleState::Enter(IEnemy* e) {

	e->PlayAnimation(IEnemy::EnAnimation::enIdle);

#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Enter enemy teleportation\n");
#endif
}

IEnemyState* EnemyMeleeIdleState::Update(IEnemy* e) {

	//“K“–‚É“®‚©‚µ‚Ä‚İ‚½‚¾‚¯
	static float time = 0.f;
	time += gameTime()->GetDeltaTime();
	auto add = std::sin(time);
	Vector3 vel = Vector3::Zero;
	vel.x = add;
	vel.y -= 10.f;
	//e->SetVelocity(vel);


	auto player = EnemyManager::GetEnemyManager().GetPlayer();
	auto& epos = e->GetPosition();
	auto& ppos = player->GetPosition();
	auto vecToPlayer = ppos - epos;
	const float chaseRange = 60.f;

	if (vecToPlayer.Length() < chaseRange and player->GetCurrentHP() > 0.f) {
		return e->GetState(IEnemy::EnState::enBattleState);
	}

	//ó‘Ô‚ª•Ï‚í‚ç‚È‚¢‚È‚ç©•ª‚ğ•Ô‚·
	return this;
}

void EnemyMeleeIdleState::Exit(IEnemy* e) {
#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Exit enemy teleportation\n");
#endif
}