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
	auto& p = GameManager::GetInstance().m_player;
	auto v = p->GetPosition() - e->GetPosition();
	v.y = 0.f;
	float vlen = v.Length();
	v /= vlen;

	auto f = e->GetForward();

	auto t = acosf(v.Dot(f));
	if (vlen < 5.f && t < Math::PI*0.5f)
		p->ApplyDamage(m_damage, true, f);

	
}

IEnemyState* EnemyShortRangePunchState::Update(IEnemy* e)
{
	if (!e->GetModel()->IsAnimPlaying())
	{
		return e->GetState(TO_INT(Zako_ShortRangeMonster::EnStateEX::enComing));
	}
	return this;
}

void EnemyShortRangePunchState::Exit(IEnemy* e)
{
}
