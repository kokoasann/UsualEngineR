#include "stdafx.h"
#include "Boss_FatmanDeadState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "../../Drop/DropItemFat.h"
#include "GameManager.h"
#include "Game.h"
#include "Effect/ExplosionEffect.h"

Boss_FatmanDeadState::Boss_FatmanDeadState()
{
}

Boss_FatmanDeadState::~Boss_FatmanDeadState()
{
	auto item = NewGO<DropItemFat>(0);
	item->SetPosition(m_pos);
}

void Boss_FatmanDeadState::Enter(IEnemy* e)
{

	m_pos = e->GetPosition();
	if (!m_isPerformed) {
		GameManager::GetInstance().m_gameScene->OnEnemyDied(e);
		m_isPerformed = true;
	}
}

IEnemyState* Boss_FatmanDeadState::Update(IEnemy* e)
{
	return this;
}

void Boss_FatmanDeadState::Exit(IEnemy* e)
{
}

void Boss_FatmanDeadState::OnAttacked(IEnemy* e)
{
}
