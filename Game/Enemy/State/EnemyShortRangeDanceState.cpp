#include "stdafx.h"
#include "EnemyShortRangeDanceState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "GameManager.h"
#include "Enemy/Zako/Zako_ShortRangeMonster.h"

EnemyShortRangeDanceState::EnemyShortRangeDanceState()
{
}

EnemyShortRangeDanceState::~EnemyShortRangeDanceState()
{
}

void EnemyShortRangeDanceState::Enter(IEnemy* e)
{
	e->PlayAnimation(TO_INT(Zako_ShortRangeMonster::EnAnimEX::enDance));
}

IEnemyState* EnemyShortRangeDanceState::Update(IEnemy* e)
{
	//一定時間経ったらステートを戻す。
	if (!e->GetModel()->IsAnimPlaying())
	{
		return e->GetState(TO_INT(Zako_ShortRangeMonster::EnStateEX::enComing));
	}
	return this;
}

void EnemyShortRangeDanceState::Exit(IEnemy* e)
{
}
