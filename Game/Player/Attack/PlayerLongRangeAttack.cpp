#include "stdafx.h"
#include "PlayerLongRangeAttack.h"
#include "../Player.h"
#include "../Pod/Pod.h"
#include "Projectile.h"
#include "../../Enemy/IEnemy.h"
#include "../../GameManager.h"
#include "../../Camera/GameCamera.h"
#include "../../Enemy/EnemyManager.h"
#include "Effect/MuzzleFlash.h"

PlayerLongRangeAttack::PlayerLongRangeAttack() {

	MuzzleFlashEffectInitData mfid;

	for (int i = 0; i < TO_INT(EnMuzzles::NumMuzzles); i++) {
		auto muzzleFlash = NewGO<MuzzleFlash>(0);
		muzzleFlash->Init(mfid);
		muzzleFlash->SetSca(Vector3::One * 0.1);
		m_muzzleFlashes.push_back(muzzleFlash);
	}

}

PlayerLongRangeAttack::~PlayerLongRangeAttack() {

}

void PlayerLongRangeAttack::Execute(Player* p) {
	auto delta = gameTime()->GetDeltaTime();
	m_shotIntervalTimer += delta;
	if (m_shotIntervalTimer >= m_interval) {
		DebugPrint_WATA("distant attacking\n");

		//p->GetPod().

		//Enemy_Bullet* eb = NewGO<Enemy_Bullet>(0, true);
		//eb->Init(p, 0.25f, m_velocity, m_speed, 5, 1);


		auto targetE = EnemyManager::GetEnemyManager().GetTargettingEnemy();

		Vector3 vel = Vector3::Zero;
		if (targetE == nullptr) {
			vel = p->GetForward();
		}
		else {
			auto vecPodToEnemy = targetE->GetPosition() - p->GetPod()->GetPosition();
			vecPodToEnemy.Normalize();
			vel = vecPodToEnemy;
		}

		auto projectileR1 = NewGO<Projectile>(0, true);
		auto projectileR2 = NewGO<Projectile>(0, true);
		auto projectileL1 = NewGO<Projectile>(0, true);
		auto projectileL2 = NewGO<Projectile>(0, true);

		//projectile->Init(p->GetPod()->GetPosition(), vel);
		vel.Normalize();
		const float defSpeed = 200.f;
		const float lifeSpan = 5.f;
		const float range = 10.f;
		const float scale = 0.2f;
		const float flashScale = 0.02f;

		float speed = defSpeed + p->GetVelocity().Length();

		auto& bones = p->GetPod()->GetBones();
		auto pvel = p->GetVelocity() * delta;

		auto posR1 = bones.at(TO_INT(Pod::EnPodBone::Burrel_R1))->GetWorldMatrix().GetTransrate() + pvel;
		auto rotR1 = bones.at(TO_INT(Pod::EnPodBone::Burrel_R1))->GetWorldMatrix().GetRotate();
		projectileR1->Init(posR1, scale, vel, speed, lifeSpan, range);
		m_muzzleFlashes.at(TO_INT(EnMuzzles::UpperRight))->Play();
		m_muzzleFlashes.at(TO_INT(EnMuzzles::UpperRight))->SetPos(posR1);
		m_muzzleFlashes.at(TO_INT(EnMuzzles::UpperRight))->SetRot(rotR1);
		m_muzzleFlashes.at(TO_INT(EnMuzzles::UpperRight))->SetSca(Vector3::One * flashScale);

		auto posR2 = bones.at(TO_INT(Pod::EnPodBone::Burrel_R2))->GetWorldMatrix().GetTransrate() + pvel;
		auto rotR2 = bones.at(TO_INT(Pod::EnPodBone::Burrel_R2))->GetWorldMatrix().GetRotate();
		projectileR2->Init(posR2, scale, vel, speed, lifeSpan, range);
		m_muzzleFlashes.at(TO_INT(EnMuzzles::UpperLeft))->Play();
		m_muzzleFlashes.at(TO_INT(EnMuzzles::UpperLeft))->SetPos(posR2);
		m_muzzleFlashes.at(TO_INT(EnMuzzles::UpperLeft))->SetRot(rotR2);
		m_muzzleFlashes.at(TO_INT(EnMuzzles::UpperLeft))->SetSca(Vector3::One * flashScale);

		auto posL1 = bones.at(TO_INT(Pod::EnPodBone::Burrel_L1))->GetWorldMatrix().GetTransrate() + pvel;
		auto rotL1 = bones.at(TO_INT(Pod::EnPodBone::Burrel_L1))->GetWorldMatrix().GetRotate();
		projectileL1->Init(posL1, scale, vel, speed, lifeSpan, range);
		m_muzzleFlashes.at(TO_INT(EnMuzzles::LowerRight))->Play();
		m_muzzleFlashes.at(TO_INT(EnMuzzles::LowerRight))->SetPos(posL1);
		m_muzzleFlashes.at(TO_INT(EnMuzzles::LowerRight))->SetRot(rotL1);
		m_muzzleFlashes.at(TO_INT(EnMuzzles::LowerRight))->SetSca(Vector3::One * flashScale);

		auto posL2 = bones.at(TO_INT(Pod::EnPodBone::Burrel_L2))->GetWorldMatrix().GetTransrate() + pvel;
		auto rotL2 = bones.at(TO_INT(Pod::EnPodBone::Burrel_L2))->GetWorldMatrix().GetRotate();
		projectileL2->Init(posL2, scale, vel, speed, lifeSpan, range);
		m_muzzleFlashes.at(TO_INT(EnMuzzles::LowerLeft))->Play();
		m_muzzleFlashes.at(TO_INT(EnMuzzles::LowerLeft))->SetPos(posL2);
		m_muzzleFlashes.at(TO_INT(EnMuzzles::LowerLeft))->SetRot(rotL2);
		m_muzzleFlashes.at(TO_INT(EnMuzzles::LowerLeft))->SetSca(Vector3::One * flashScale);

		m_shotIntervalTimer = 0.f;
	}
}
