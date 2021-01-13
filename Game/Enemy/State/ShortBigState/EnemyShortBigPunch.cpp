#include "stdafx.h"
#include "EnemyShortBigPunch.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Zako/Zako_ShortBig.h"

EnemyShortBigPunch::EnemyShortBigPunch()
{
}

EnemyShortBigPunch::~EnemyShortBigPunch()
{
}

void EnemyShortBigPunch::Enter(IEnemy* e)
{
	e->PlayAnimation(IEnemy::EnAnimation::enAttackA);
	auto& p = GameManager::GetInstance().m_player;
	auto v = p->GetPosition() - e->GetPosition();
	v.y = 0.f;
	float vlen = v.Length();
	v /= vlen;
	
	auto f = e->GetForward();

	auto t = acosf(v.Dot(f));

	const float BigShortVector = 125.f;

	//���݋N���Ă�����
	//�_���[�W��^����ꂽ�Ƃ��A
	//���O�Ƀ_���[�W�^����ꂽ���̃x�N�g�����Q�Ƃ��Đ������ł���
	//�Ȃ̂ŁA1��ڂ̎��͔�΂���
	//2��ڂ̎��ɉ�����ƁA���ڂɂȂ���ꂽ�x�N�g���̕����ɔ��
	//�H�H
	if (vlen < 25.f && t < Math::PI * 0.5f) {
		p->ApplyDamage(m_damage, true, f * BigShortVector);
	}
}

IEnemyState* EnemyShortBigPunch::Update(IEnemy* e)
{
	if (!e->GetModel()->IsAnimPlaying())
	{
		return e->GetState(TO_INT(Zako_ShortBig::EnStateEX::enComing));
	}
	return this;
}

void EnemyShortBigPunch::Exit(IEnemy* e)
{
}

void EnemyShortBigPunch::OnAttacked(IEnemy* e)
{
}
