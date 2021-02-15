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
	for (int i = 0; i < m_muzzleFlashes.size(); i++) {
		DeleteGO(m_muzzleFlashes.at(i));
	}
}

void PlayerLongRangeAttack::Execute(Player* p) {
	auto delta = gameTime()->GetDeltaTime();
	m_shotIntervalTimer += delta;

	auto& bones = p->GetPod()->GetBones();
	auto pvel = p->GetVelocity() * delta;

	if (m_shotIntervalTimer >= m_interval) {

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

		auto projectileR1 = NewGO<Projectile>(1, true);
		auto projectileR2 = NewGO<Projectile>(1, true);
		auto projectileL1 = NewGO<Projectile>(1, true);
		auto projectileL2 = NewGO<Projectile>(1, true);

		//projectile->Init(p->GetPod()->GetPosition(), vel);
		vel.Normalize();
		const float defSpeed = 800.f;
		const float lifeSpan = 5.f;
		const float range = 10.f;
		const float scale = 0.2f;
		const float flashScale = 0.02f;

		float speed = defSpeed + p->GetVelocity().Length();

		projectileR1->Init(m_effectTransforms.m_posList[TO_INT(Pod::EnPodBone::Burrel_R1)] - pvel, scale, vel, speed, lifeSpan, range);
		m_muzzleFlashes.at(TO_INT(EnMuzzles::UpperRight))->Play();
		m_muzzleFlashes.at(TO_INT(EnMuzzles::UpperRight))->SetSca(Vector3::One * flashScale);

		projectileR2->Init(m_effectTransforms.m_posList[TO_INT(Pod::EnPodBone::Burrel_R2)] - pvel, scale, vel, speed, lifeSpan, range);
		m_muzzleFlashes.at(TO_INT(EnMuzzles::UpperLeft))->Play();
		m_muzzleFlashes.at(TO_INT(EnMuzzles::UpperLeft))->SetSca(Vector3::One * flashScale);

		projectileL1->Init(m_effectTransforms.m_posList[TO_INT(Pod::EnPodBone::Burrel_L1)] - pvel, scale, vel, speed, lifeSpan, range);
		m_muzzleFlashes.at(TO_INT(EnMuzzles::LowerRight))->Play();
		m_muzzleFlashes.at(TO_INT(EnMuzzles::LowerRight))->SetSca(Vector3::One * flashScale);

		projectileL2->Init(m_effectTransforms.m_posList[TO_INT(Pod::EnPodBone::Burrel_L2)] - pvel, scale, vel, speed, lifeSpan, range);
		m_muzzleFlashes.at(TO_INT(EnMuzzles::LowerLeft))->Play();
		m_muzzleFlashes.at(TO_INT(EnMuzzles::LowerLeft))->SetSca(Vector3::One * flashScale);

		m_shotIntervalTimer = 0.f;
	}

}


void PlayerLongRangeAttack::CalcEffectsTransform() {

	auto delta = gameTime()->GetDeltaTime();
	auto p = GameManager::GetInstance().m_player;
	auto& bones = p->GetPod()->GetBones();
	auto pvel = p->GetVelocity() * delta;

	//R1
	m_effectTransforms.m_posList[TO_INT(Pod::EnPodBone::Burrel_R1)] = bones.at(TO_INT(Pod::EnPodBone::Burrel_R1))->GetWorldMatrix().GetTransrate() + pvel;
	m_effectTransforms.m_rotList[TO_INT(Pod::EnPodBone::Burrel_R1)] = bones.at(TO_INT(Pod::EnPodBone::Burrel_R1))->GetWorldMatrix().GetRotate();
	//R2
	m_effectTransforms.m_posList[TO_INT(Pod::EnPodBone::Burrel_R2)] = bones.at(TO_INT(Pod::EnPodBone::Burrel_R2))->GetWorldMatrix().GetTransrate() + pvel;
	m_effectTransforms.m_rotList[TO_INT(Pod::EnPodBone::Burrel_R2)] = bones.at(TO_INT(Pod::EnPodBone::Burrel_R2))->GetWorldMatrix().GetRotate();
	//L1
	m_effectTransforms.m_posList[TO_INT(Pod::EnPodBone::Burrel_L1)] = bones.at(TO_INT(Pod::EnPodBone::Burrel_L1))->GetWorldMatrix().GetTransrate() + pvel;
	m_effectTransforms.m_rotList[TO_INT(Pod::EnPodBone::Burrel_L1)] = bones.at(TO_INT(Pod::EnPodBone::Burrel_L1))->GetWorldMatrix().GetRotate();
	//L2
	m_effectTransforms.m_posList[TO_INT(Pod::EnPodBone::Burrel_L2)] = bones.at(TO_INT(Pod::EnPodBone::Burrel_L2))->GetWorldMatrix().GetTransrate() + pvel;
	m_effectTransforms.m_rotList[TO_INT(Pod::EnPodBone::Burrel_L2)] = bones.at(TO_INT(Pod::EnPodBone::Burrel_L2))->GetWorldMatrix().GetRotate();


}

void PlayerLongRangeAttack::UpdateEffectPos() {
	CalcEffectsTransform();
	//R1
	m_muzzleFlashes.at(TO_INT(EnMuzzles::UpperRight))->SetPos(m_effectTransforms.m_posList[TO_INT(Pod::EnPodBone::Burrel_R1)]);
	m_muzzleFlashes.at(TO_INT(EnMuzzles::UpperRight))->SetRot(m_effectTransforms.m_rotList[TO_INT(Pod::EnPodBone::Burrel_R1)]);
	//R2
	m_muzzleFlashes.at(TO_INT(EnMuzzles::UpperLeft))->SetPos(m_effectTransforms.m_posList[TO_INT(Pod::EnPodBone::Burrel_R2)]);
	m_muzzleFlashes.at(TO_INT(EnMuzzles::UpperLeft))->SetRot(m_effectTransforms.m_rotList[TO_INT(Pod::EnPodBone::Burrel_R2)]);
	//L1
	m_muzzleFlashes.at(TO_INT(EnMuzzles::LowerRight))->SetPos(m_effectTransforms.m_posList[TO_INT(Pod::EnPodBone::Burrel_L1)]);
	m_muzzleFlashes.at(TO_INT(EnMuzzles::LowerRight))->SetRot(m_effectTransforms.m_rotList[TO_INT(Pod::EnPodBone::Burrel_L1)]);
	//L2
	m_muzzleFlashes.at(TO_INT(EnMuzzles::LowerLeft))->SetPos(m_effectTransforms.m_posList[TO_INT(Pod::EnPodBone::Burrel_L2)]);
	m_muzzleFlashes.at(TO_INT(EnMuzzles::LowerLeft))->SetRot(m_effectTransforms.m_rotList[TO_INT(Pod::EnPodBone::Burrel_L2)]);

}