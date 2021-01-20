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
	v.y = 0.f;
	float vlen = v.Length();
	v /= vlen;

	auto f = e->GetForward();
	auto t = acosf(v.Dot(f));
	if (vlen < 5.f && t < Math::PI * 0.5f) {
		p->ApplyDamage(m_damage, true, f);
		//テストコード。殴打音
		CSoundSource* se = NewGO<CSoundSource>(0, "Punch");
		se->Init(L"Assets/sound/Goblin_Punch1.wav");
		se->Play(false);
		se->SetVolume(1.0f);
	}
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