#include "stdafx.h"
#include "BossBombStunState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"

BossBombStunState::BossBombStunState()
{
}

BossBombStunState::~BossBombStunState()
{
}

void BossBombStunState::Enter(IEnemy* e)
{
}

IEnemyState* BossBombStunState::Update(IEnemy* e)
{
	return this;
}

void BossBombStunState::Exit(IEnemy* e)
{
}

void BossBombStunState::OnAttacked(IEnemy* e)
{
}
