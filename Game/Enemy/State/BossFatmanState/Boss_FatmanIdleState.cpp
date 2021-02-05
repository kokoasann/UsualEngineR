#include "stdafx.h"
#include "Boss_FatmanIdleState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"

Boss_FatmanIdleState::Boss_FatmanIdleState()
{
}

Boss_FatmanIdleState::~Boss_FatmanIdleState()
{
}

void Boss_FatmanIdleState::Enter(IEnemy* e)
{
	e->PlayAnimation(IEnemy::EnAnimation::enIdle);
}

IEnemyState* Boss_FatmanIdleState::Update(IEnemy* e)
{
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
	const float chaseRange = 100.f;

	if (vecToPlayer.Length() < chaseRange and player->GetCurrentHP() > 0.f) {
		return e->GetState(TO_INT(IEnemy::EnState::enBattleState));
	}
	return this;
}

void Boss_FatmanIdleState::Exit(IEnemy* e)
{
}

void Boss_FatmanIdleState::OnAttacked(IEnemy* e)
{
}
