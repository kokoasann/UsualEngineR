#include "stdafx.h"
#include "BossBombFullFrontalState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Boss/Boss_MiddleBomb.h"
#include "GameManager.h"

BossBombFullFrontalState::BossBombFullFrontalState()
{
}

BossBombFullFrontalState::~BossBombFullFrontalState()
{
}

void BossBombFullFrontalState::Enter(IEnemy* e)
{
	e->PlayAnimation(TO_INT(Boss_MiddleBomb::EnAnimEX::FullFrontal));
}

IEnemyState* BossBombFullFrontalState::Update(IEnemy* e)
{
	return this;
}

void BossBombFullFrontalState::Exit(IEnemy* e)
{
}

void BossBombFullFrontalState::OnAttacked(IEnemy* e)
{
}
