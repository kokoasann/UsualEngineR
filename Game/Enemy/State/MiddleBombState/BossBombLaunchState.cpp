#include "stdafx.h"
#include "BossBombLaunchState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Equipment/Enemy_LaunchBomb.h"

BossBombLaunchState::BossBombLaunchState()
{
}

BossBombLaunchState::~BossBombLaunchState()
{
}

void BossBombLaunchState::Enter(IEnemy* e)
{
	e->PlayAnimation(TO_INT(IEnemy::EnAnimation::enAttackA));

}

IEnemyState* BossBombLaunchState::Update(IEnemy* e)
{
	if (e->GetModel()->IsAnimPlaying())
		return this;

	Enemy_LaunchBomb* bomb = NewGO<Enemy_LaunchBomb>(0,true);
	bomb->Init(e->GetPosition() + Vector3::Up * 50.f, (e->GetForward() + Vector3::Up) * 500.f, 2);

	return e->GetState(TO_INT(IEnemy::EnState::enBattleState));
}

void BossBombLaunchState::Exit(IEnemy* e)
{
}

void BossBombLaunchState::OnAttacked(IEnemy* e)
{
}
