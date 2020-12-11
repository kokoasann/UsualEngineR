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
	auto& p = GameManager::GetInstance().m_player;
	auto v = p->GetPosition() - e->GetPosition();
	v.Normalize();
	v *= 100.f;
	p->ApplyDamage(m_damage, true, v);
}

IEnemyState* EnemyShortRangeStrongPunchState::Update(IEnemy* e)
{
	if (!e->GetModel()->IsAnimPlaying()){
		return e->GetState(TO_INT(Zako_ShortRangeMonster::EnStateEX::enComing));
	}
	return this;
}

void EnemyShortRangeStrongPunchState::Exit(IEnemy* e)
{
}