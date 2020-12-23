#include "stdafx.h"
#include "EnemyShortBigDance.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Zako/Zako_ShortBig.h"

EnemyShortBigDance::EnemyShortBigDance()
{
}

EnemyShortBigDance::~EnemyShortBigDance()
{
}

void EnemyShortBigDance::Enter(IEnemy* e)
{
	e->PlayAnimation(TO_INT(Zako_ShortBig::EnAnimEX::enDance));
}

IEnemyState* EnemyShortBigDance::Update(IEnemy* e)
{
	if (!e->GetModel()->IsAnimPlaying())
	{
		return e->GetState(TO_INT(Zako_ShortBig::EnStateEX::enComing));
	}
	return this;
}

void EnemyShortBigDance::Exit(IEnemy* e)
{
}

void EnemyShortBigDance::OnAttacked(IEnemy* e)
{
}
