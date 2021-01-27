#include "stdafx.h"
#include "PlayerAttackHipShot2.h"
#include "../../Player.h"
#include "../../../Enemy/EnemyManager.h"
#include "../Projectile.h"
#include "Player/Attachment/Gun.h"

PlayerAttackHipShot2::PlayerAttackHipShot2() {

}

PlayerAttackHipShot2::~PlayerAttackHipShot2() {

}

//void PlayerAttackHipShot2::Execute(Player* player) {
//
//	//const auto delta = gameTime()->GetDeltaTime();
//	//auto lxf = g_pad[0]->GetLStickXF();
//	//Quaternion rot = Quaternion::Identity;
//	//const float rotationSpeed = 100.f;
//	//rot.SetRotationDegY(lxf * rotationSpeed * delta);
//	//auto prot = player->GetRotation();
//	//prot.Multiply(rot);
//	//player->SetRotation(prot);
//	GenerateProjectiles(player);
//
//}

void PlayerAttackHipShot2::GenerateProjectiles(Player* player) {
	const auto deltaTime = gameTime()->GetDeltaTime();
	m_shotIntervalTimer += deltaTime;
	if (m_shotIntervalTimer >= m_shotInterval) {

		auto projectileL = NewGO<Projectile>(0, true);
		auto projectileR = NewGO<Projectile>(0, true);

		auto vel = player->GetForward();
		vel.y = 0.f;

		//
		const float defSpeed = 400.f;
		const float lifeSpan = 10.f;
		const float range = 10.f;
		const float scale = 0.3f;

		const auto posLeft = player->GetGun()->GetGunModelBone(Gun::GunBone::Left)->GetWorldMatrix().GetTransrate();
		const auto posRight = player->GetGun()->GetGunModelBone(Gun::GunBone::Right)->GetWorldMatrix().GetTransrate();
		const auto& pvel = player->GetVelocity() *deltaTime;

		projectileL->Init(posLeft +pvel , scale, vel, defSpeed, lifeSpan, range);
		projectileR->Init(posRight + pvel, scale, vel, defSpeed, lifeSpan, range);

		//projectile->Init(player->GetPosition(), vel);

		//Effect
		player->GetGun()->PlayFireEffects();

		m_shotIntervalTimer = 0.f;

		auto se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/chara/shoot.wav", true);
		se->Play(false);

	}
}

