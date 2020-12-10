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

	//一定間隔で弾を撃つ。
	//if (m_shotTimer > shotTime) {
		//撃ちだす方向を決める。
		//少し上に設定。
		const float upY = 6.f;
		Enemy_Bullet* eb = NewGO<Enemy_Bullet>(0, true);
		auto epos = e->GetPosition();
		epos.y += upY;

		auto& p = GameManager::GetInstance().m_player;
		auto ppos = p->GetPosition();
		ppos.y += upY;
		
		Vector3 dir = ppos - epos;
		dir.Normalize();

		//撃ちだす弾の振れ幅を決める。(1つ目の回転)
		Vector3 cross;
		cross.Cross(dir, Vector3::Up);
		
		//ランダムな角度。
		const float maxAngle = 5.f;
		float angle = 0.f;
		auto rand = GRandom().Rand();
		angle = maxAngle * rand;

		//1つ目の回転を作成。
		Quaternion rot;
		rot.SetRotationDeg(cross, angle);

		//主軸を回転させる。(2つ目の回転)
		//ランダムな角度。
		const float maxAngle2 = 360.f;
		float angle2 = 0.f;
		auto rand2 = GRandom().Rand();
		angle2 = maxAngle2 * rand2;

		//2つ目の回転を作成。
		Quaternion rot2;
		rot2.SetRotationDeg(dir, angle2);

		//回転を適用。
		rot.Apply(dir);
		rot2.Apply(dir);

		//弾の初期化。
		eb->Init(epos, 0.6f, dir, 400.f, 5.f, 2.f);
		eb->SetDamage(m_damage);
		m_shotTimer = 0.f;
	//}
	return this;
}

void Boss_FatmanShootingState::Exit(IEnemy* e)
{
}
