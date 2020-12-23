#include "stdafx.h"
#include "EnemyShortBigStrongPunch.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Zako/Zako_ShortBig.h"

EnemyShortBigStrongPunch::EnemyShortBigStrongPunch()
{
}

EnemyShortBigStrongPunch::~EnemyShortBigStrongPunch()
{
}

void EnemyShortBigStrongPunch::Enter(IEnemy* e)
{
	e->PlayAnimation(IEnemy::EnAnimation::enAttackB);
	auto& p = GameManager::GetInstance().m_player;
	auto v = p->GetPosition() - e->GetPosition();
	v.y = 0.f;
	float vlen = v.Length();
	v /= vlen;

	auto f = e->GetForward();
	auto t = acosf(v.Dot(f));
	if (vlen < 5.f && t < Math::PI * 0.5f)
		p->ApplyDamage(m_damage, true, f);
}

IEnemyState* EnemyShortBigStrongPunch::Update(IEnemy* e)
{
	if (!e->GetModel()->IsAnimPlaying()) {
		return e->GetState(TO_INT(Zako_ShortBig::EnStateEX::enComing));
	}
	return this;
}

void EnemyShortBigStrongPunch::Exit(IEnemy* e)
{
}

void EnemyShortBigStrongPunch::OnAttacked(IEnemy* e)
{
}
