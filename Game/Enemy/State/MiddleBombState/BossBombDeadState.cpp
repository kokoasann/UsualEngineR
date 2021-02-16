#include "stdafx.h"
#include "BossBombDeadState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Boss/Boss_MiddleBomb.h"
#include "Enemy/BossBGM.h"
#include "../../Drop/DropItemBom.h"
#include "GameManager.h"
#include "Game.h"

BossBombDeadState::BossBombDeadState()
{
}

BossBombDeadState::~BossBombDeadState()
{
}

void BossBombDeadState::Enter(IEnemy* e)
{
	m_pos = e->GetPosition();
	GameManager::GetInstance().m_bgm->SetInBattleFlag(IEnemy::EnBossType::Bomb, false);
	GameManager::GetInstance().m_gameScene->OnEnemyDied(e);
}

IEnemyState* BossBombDeadState::Update(IEnemy* e)
{
	return this;
}

void BossBombDeadState::Exit(IEnemy* e)
{
}

void BossBombDeadState::OnAttacked(IEnemy* e)
{
}
