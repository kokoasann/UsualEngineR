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
	m_forward = GameManager::GetInstance().m_player->GetPosition() - e->GetPosition();
	m_forward.y = 0;
	m_forward.Normalize();
	m_forward *= m_fowardMove;

	e->PlayAnimation(IEnemy::EnAnimation::enAttackB);
	auto& p = GameManager::GetInstance().m_player;
	auto v = p->GetPosition() - e->GetPosition();
	v.y = 0.f;
	float vlen = v.Length();
	v /= vlen;

	const float BigShortVector = 125.0f;

	auto f = e->GetForward();
	auto t = acosf(v.Dot(f));

	if (vlen < 25.f && t < Math::PI * 0.5f) {
		//hukitobiVec = m_forward;
		//殴打音。
		CSoundSource* se = NewGO<CSoundSource>(0,"StrongPunch");
		se->Init(L"Assets/sound/Goblin_StrongPunch1.wav");
		se->Play(false);
		se->SetVolume(1.0f);
		//ダメージとnkの適応
		p->ApplyDamage(m_damage, true, f * BigShortVector);
	}
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
