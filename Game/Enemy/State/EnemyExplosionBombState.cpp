#include "stdafx.h"
#include "EnemyExplosionBombState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "GameManager.h"

EnemyExplosionBombState::EnemyExplosionBombState()
{
}

EnemyExplosionBombState::~EnemyExplosionBombState()
{
}

void EnemyExplosionBombState::Enter(IEnemy* e)
{
	auto& p = GameManager::GetInstance().m_player;
	auto& ppos = p->GetPosition();
	auto& epos = e->GetPosition();
	auto vecToPlayer = ppos - epos;
	const float exprange = 30.0f;

	if (vecToPlayer.Length() < exprange) {
		auto& p = GameManager::GetInstance().m_player;
		p->ApplyDamage(m_damage);
	}
}

IEnemyState* EnemyExplosionBombState::Update(IEnemy* e)
{
	//‚Æ‚è‚ ‚¦‚¸B
	return e->GetState(TO_INT(IEnemy::EnState::enDeadState));
	//return this;
}

void EnemyExplosionBombState::Exit(IEnemy* e)
{
}
