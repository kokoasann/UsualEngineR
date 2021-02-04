#include "stdafx.h"
#include "Boss_FatmanShootingState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "GameManager.h"
#include "Enemy/Equipment/Enemy_Bullet.h"
#include "Enemy/Boss/Boss_Fatman.h"
#include "Effect/MuzzleFlash.h"

Boss_FatmanShootingState::Boss_FatmanShootingState()
{
	MuzzleFlashEffectInitData mfid;
	for (int i = 0; i < IKNum; i++) {
		auto muzzleFlash = NewGO<MuzzleFlash>(0);
		muzzleFlash->Init(mfid);
		muzzleFlash->SetSca(Vector3::One * 0.1);
		m_muzzleFlashes.push_back(muzzleFlash);
	}
	m_shootSE = NewGO<CSoundSource>(0);
	m_shootSE->Init(L"Assets/sound/chara/mg.wav");
}

Boss_FatmanShootingState::~Boss_FatmanShootingState()
{
	DeleteGO(m_shootSE);
}

void Boss_FatmanShootingState::Enter(IEnemy* e)
{
	const float time = 20.f;
	m_damage = Boss_Fatman::CalcDamage(time);

	m_shotTimer = 0.f;
	m_stateTimer = 0.f;

	//IK情報。
	for (int i = 0; i < IKNum; i++) {
		m_ik[i] = e->GetIK(TO_INT(IEnemy::EnIK::enFoot_L) + i);
	}
}

IEnemyState* Boss_FatmanShootingState::Update(IEnemy* e)
{
	e->GetModel()->SetRotation(Boss_Fatman::EnemyToPlayerRotation(e));

	const float shotTime = 0.01f;
	m_shotTimer += gameTime()->GetDeltaTime();
	m_stateTimer += gameTime()->GetDeltaTime();
	const float maxTime = 5.f;
	if (m_stateTimer > maxTime) {
		m_isKnockBack = false;
		m_shootSE->Stop();
		return e->GetState(TO_INT(IEnemy::EnState::enBattleState));
	}

	//一定間隔で弾を撃つ。
	if (m_shotTimer > shotTime) {
		auto rand = GRandom().Rand();
		rand *= 3.f;
		for (int i = 0; i <= rand; i++){
			BulletGenerate(e);
			m_shootSE->Play(true);
		}
		
		m_shotTimer = 0.f;
	}

	//マズルフラッシュ。
	for (int ikNo = 0; ikNo < IKNum; ikNo++) {
		m_muzzleFlashes[ikNo]->SetPos(m_ik[ikNo]->GetEffectorBone()->GetWorldMatrix().GetTransrate());
		m_muzzleFlashes[ikNo]->SetRot(m_ik[ikNo]->GetEffectorBone()->GetWorldMatrix().GetRotate());
		m_muzzleFlashes[ikNo]->Play();
	}
	return this;
}

void Boss_FatmanShootingState::Exit(IEnemy* e)
{
}

void Boss_FatmanShootingState::BulletGenerate(IEnemy* e)
{
	for (int i = 0; i < IKNum; i++) {
		//撃ちだす方向を決める。
		//少し上に設定。
		const float upY = 6.f;
		Enemy_Bullet* eb = NewGO<Enemy_Bullet>(0, true);
		/*auto epos = e->GetPosition();
		epos.y += upY;*/
		Vector3 epos = m_ik[i]->GetEffectorBone()->GetWorldMatrix().GetTransrate();

		auto& p = GameManager::GetInstance().m_player;
		auto ppos = p->GetPosition();
		ppos.y += 6.0f;

		for (int i = 0; i < IKNum; i++) {
			m_ik[i]->SetNextTarget(ppos);
		}

		Vector3 dir = ppos - epos;
		dir.Normalize();

		//撃ちだす弾の振れ幅を決める。(1つ目の回転)
		Vector3 cross;
		cross.Cross(dir, Vector3::Up);

		//ランダムな角度。
		const float maxAngle = 4.f;
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

		//攻撃。
		//プレイヤーが飛んでいたら撃ち落とす。
		Vector3 knockVector = ppos - epos;
		knockVector.Normalize();
		const float knockParam = 200.f;
		knockVector *= knockParam;
		if (!m_isKnockBack && !p->IsOnGround()) {
			eb->SetDamage(m_damage, true, knockVector);
			m_isKnockBack = false;
		}
		else {
			eb->SetDamage(m_damage);
		}
	}
}
