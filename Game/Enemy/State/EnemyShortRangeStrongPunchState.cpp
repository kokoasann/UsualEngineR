#include "stdafx.h"
#include "EnemyShortRangeStrongPunchState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "GameManager.h"
#include "Enemy/Zako/Zako_ShortRangeMonster.h"

EnemyShortRangeStrongPunchState::EnemyShortRangeStrongPunchState()
{
}

EnemyShortRangeStrongPunchState::~EnemyShortRangeStrongPunchState()
{
}

void EnemyShortRangeStrongPunchState::Enter(IEnemy* e)
{
	e->PlayAnimation(IEnemy::EnAnimation::enAttackB);
}

IEnemyState* EnemyShortRangeStrongPunchState::Update(IEnemy* e)
{
	if (!e->GetModel()->IsAnimPlaying()){
		auto& p = GameManager::GetInstance().m_player;
		p->ApplyDamage(m_damage);
		return e->GetState(TO_INT(Zako_ShortRangeMonster::EnStateEX::enComing));
	}
	return this;
}

void EnemyShortRangeStrongPunchState::Exit(IEnemy* e)
{
}