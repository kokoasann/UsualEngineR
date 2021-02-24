#include "stdafx.h"
#include "Boss_FatmanIdleState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "GameManager.h"
#include "Game.h"
#include "Enemy/BossBGM.h"
Boss_FatmanIdleState::Boss_FatmanIdleState()
{
}

Boss_FatmanIdleState::~Boss_FatmanIdleState()
{
}

void Boss_FatmanIdleState::Enter(IEnemy* e)
{
	e->PlayAnimation(IEnemy::EnAnimation::enIdle);
	if (GameManager::GetInstance().m_bgm != nullptr) {
		//GameManager::GetInstance().m_bgm->AddBgmCount(-1);
		GameManager::GetInstance().m_bgm->SetInBattleFlag(IEnemy::EnBossType::Fat, false);
	}
}

IEnemyState* Boss_FatmanIdleState::Update(IEnemy* e)
{
	auto player = GameManager::GetInstance().m_player;
	auto& epos = e->GetPosition();
	auto& ppos = player->GetPosition();
	auto vecToPlayer = ppos - epos;
	const float chaseRange = 100.f;

	if (vecToPlayer.Length() < chaseRange and player->GetCurrentHP() > 0.f) {
		if (GameManager::GetInstance().m_bgm != nullptr) {
			GameManager::GetInstance().m_bgm->SetInBattleFlag(IEnemy::EnBossType::Fat, true);
		}
		return e->GetState(TO_INT(IEnemy::EnState::enStunState));
	}
	return this;
}

void Boss_FatmanIdleState::Exit(IEnemy* e)
{
}

void Boss_FatmanIdleState::OnAttacked(IEnemy* e)
{
}
