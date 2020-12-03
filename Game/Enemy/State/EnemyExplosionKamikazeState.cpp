#include "stdafx.h"
#include "EnemyExplosionKamikazeState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "GameManager.h"

EnemyExplosionKamikazeState::EnemyExplosionKamikazeState()
{
}

EnemyExplosionKamikazeState::~EnemyExplosionKamikazeState()
{
}

void EnemyExplosionKamikazeState::Enter(IEnemy* e)
{
}

IEnemyState* EnemyExplosionKamikazeState::Update(IEnemy* e)
{
	auto& p = GameManager::GetInstance().m_player;
	auto& ppos = p->GetPosition();
	auto& epos = e->GetPosition();
	auto vecToPlayer = ppos - epos;
	const float exprange = 10.0f;

	m_bombTimer += gameTime()->GetDeltaTime();
	const float maxTime = 5.0f;

	if (vecToPlayer.Length() < exprange or m_bombTimer > maxTime){
		
		return e->GetState(TO_INT(IEnemy::EnState::enAttackA));
	}

	Move(e);

	return this;
}

void EnemyExplosionKamikazeState::Exit(IEnemy* e)
{
}

void EnemyExplosionKamikazeState::Move(IEnemy* e)
{
	auto& p = GameManager::GetInstance().m_player;
	auto& ppos = p->GetPosition();
	auto& epos = e->GetPosition();
	auto vecToPlayer = ppos - epos;
	vecToPlayer.Normalize();

	const float walkSpeed = 100.0f;
	Vector3 moveSpeed = vecToPlayer * walkSpeed;

	e->SetVelocity(moveSpeed);
}
