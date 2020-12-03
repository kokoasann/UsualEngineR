#include "stdafx.h"
#include "EnemyShortRangePunchState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "GameManager.h"
#include "Enemy/Zako/Zako_ShortRangeMonster.h"

EnemyShortRangePunchState::EnemyShortRangePunchState()
{
}

EnemyShortRangePunchState::~EnemyShortRangePunchState()
{
}

void EnemyShortRangePunchState::Enter(IEnemy* e)
{
	e->PlayAnimation(IEnemy::EnAnimation::enAttackA);
}

IEnemyState* EnemyShortRangePunchState::Update(IEnemy* e)
{
	if (!e->GetModel()->IsAnimPlaying())
	{
		auto& p = GameManager::GetInstance().m_player;
		p->ApplyDamage(m_damage);
		return e->GetState(TO_INT(Zako_ShortRangeMonster::EnStateEX::enComing));
	}
	return this;
}

void EnemyShortRangePunchState::Exit(IEnemy* e)
{
}
