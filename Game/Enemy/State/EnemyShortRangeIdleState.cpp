#include "stdafx.h"
#include "EnemyShortRangeIdleState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"

EnemyShortRangeIdleState::EnemyShortRangeIdleState()
{
}

EnemyShortRangeIdleState::~EnemyShortRangeIdleState()
{
}

void EnemyShortRangeIdleState::Enter(IEnemy* e)
{
	e->PlayAnimation(IEnemy::EnAnimation::enIdle);

	//重力。
	const Vector3 gravity = { 0.f,-5000.f,0.f };
	e->SetVelocity(gravity);
}

IEnemyState* EnemyShortRangeIdleState::Update(IEnemy* e)
{
	auto player = GameManager::GetInstance().GetPlayer();

	auto& epos = e->GetPosition();
	auto& ppos = player->GetPosition();

	//プレイヤーが空中にいるなら動かない。
	const float airPlayerYPos = 70.0f;
	if (ppos.y > airPlayerYPos) {
		return this;
	}

	auto vecToPlayer = ppos - epos;
	const float chaseRange = 100.f;

	if (vecToPlayer.Length() < chaseRange and player->GetCurrentHP() > 0.f) {
		return e->GetState(TO_INT(IEnemy::EnState::enBattleState));
	}

	return this;
}

void EnemyShortRangeIdleState::Exit(IEnemy* e)
{
}
