#include "stdafx.h"
#include "Boss_FatmanShootingState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "GameManager.h"
#include "Enemy/Equipment/Enemy_Bullet.h"
#include "Enemy/Boss/Boss_Fatman.h"

Boss_FatmanShootingState::Boss_FatmanShootingState()
{
}

Boss_FatmanShootingState::~Boss_FatmanShootingState()
{
}

void Boss_FatmanShootingState::Enter(IEnemy* e)
{
	const float time = 20.f;
	m_damage = Boss_Fatman::CalcDamage(time);
}

IEnemyState* Boss_FatmanShootingState::Update(IEnemy* e)
{
	const float shotTime = 0.05f;
	m_shotTimer += gameTime()->GetDeltaTime();

	//���Ԋu�Œe�����B
	//if (m_shotTimer > shotTime) {
		//�����������������߂�B
		//������ɐݒ�B
		const float upY = 6.f;
		Enemy_Bullet* eb = NewGO<Enemy_Bullet>(0, true);
		auto epos = e->GetPosition();
		epos.y += upY;

		auto& p = GameManager::GetInstance().m_player;
		auto ppos = p->GetPosition();
		ppos.y += upY;
		
		Vector3 dir = ppos - epos;
		dir.Normalize();

		//���������e�̐U�ꕝ�����߂�B(1�ڂ̉�])
		Vector3 cross;
		cross.Cross(dir, Vector3::Up);
		
		//�����_���Ȋp�x�B
		const float maxAngle = 5.f;
		float angle = 0.f;
		auto rand = GRandom().Rand();
		angle = maxAngle * rand;

		//1�ڂ̉�]���쐬�B
		Quaternion rot;
		rot.SetRotationDeg(cross, angle);

		//�厲����]������B(2�ڂ̉�])
		//�����_���Ȋp�x�B
		const float maxAngle2 = 360.f;
		float angle2 = 0.f;
		auto rand2 = GRandom().Rand();
		angle2 = maxAngle2 * rand2;

		//2�ڂ̉�]���쐬�B
		Quaternion rot2;
		rot2.SetRotationDeg(dir, angle2);

		//��]��K�p�B
		rot.Apply(dir);
		rot2.Apply(dir);

		//�e�̏������B
		eb->Init(epos, 0.6f, dir, 400.f, 5.f, 2.f);
		eb->SetDamage(m_damage);
		m_shotTimer = 0.f;
	//}
	return this;
}

void Boss_FatmanShootingState::Exit(IEnemy* e)
{
}
