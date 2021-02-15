#include "stdafx.h"
#include "Boss_FatmanDeadState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/BossBGM.h"
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
}

void Boss_FatmanDeadState::Enter(IEnemy* e)
{
	GameManager::GetInstance().m_bgm->SetInBattleFlag(IEnemy::EnBossType::Fat, false);

	m_pos = e->GetPosition();
	if (!m_isPerformed) {
		GameManager::GetInstance().m_gameScene->OnEnemyDied(e);
		CSoundSource* se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/Boss_Shout.wav");
		se->Play(false);
		se->SetVolume(1.0f);
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
