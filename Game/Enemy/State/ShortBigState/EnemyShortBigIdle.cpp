#include "stdafx.h"
#include "EnemyShortBigIdle.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Zako/Zako_ShortBig.h"

EnemyShortBigIdle::EnemyShortBigIdle()
{
}

EnemyShortBigIdle::~EnemyShortBigIdle()
{
}

void EnemyShortBigIdle::Enter(IEnemy* e)
{
	e->PlayAnimation(IEnemy::EnAnimation::enIdle);

	//重力。
	const Vector3 gravity = { 0.f,-5000.f,0.f };
	e->SetVelocity(gravity);
}

IEnemyState* EnemyShortBigIdle::Update(IEnemy* e)
{
	auto player = GameManager::GetInstance().GetPlayer();

	auto& epos = e->GetPosition();
	auto& ppos = player->GetPosition();
	auto vecToPlayer = ppos - epos;

	//プレイヤーが空中にいるなら動かない。
	const float airPlayerYPos = 70.0f;
	if (vecToPlayer.y > airPlayerYPos) {
		return this;
	}

	const float chaseRange = 100.f;

	if ((vecToPlayer.Length() < chaseRange and player->GetCurrentHP() > 0.f) or m_isAttacked) {
		return e->GetState(TO_INT(IEnemy::EnState::enBattleState));
	}

	return this;
}

void EnemyShortBigIdle::Exit(IEnemy* e)
{
}
