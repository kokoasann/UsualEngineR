#include "stdafx.h"
#include "Boss_FatmanStunState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"

Boss_FatmanStunState::Boss_FatmanStunState()
{
}

Boss_FatmanStunState::~Boss_FatmanStunState()
{
}

void Boss_FatmanStunState::Enter(IEnemy* e)
{
}

IEnemyState* Boss_FatmanStunState::Update(IEnemy* e)
{
	return e->GetState(TO_INT(IEnemy::EnState::enStunState));
}

void Boss_FatmanStunState::Exit(IEnemy* e)
{
}

void Boss_FatmanStunState::OnAttacked(IEnemy* e)
{
}
