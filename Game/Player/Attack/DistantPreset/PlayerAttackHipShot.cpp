#include "stdafx.h"
#include "PlayerAttackHipShot.h"
#include "../../Player.h"
#include "../../../Enemy/EnemyManager.h"
#include "../Projectile.h"
#include "Player/Attachment/Gun.h"

PlayerAttackHipShot::PlayerAttackHipShot() {
}

PlayerAttackHipShot::~PlayerAttackHipShot() {

}

void PlayerAttackHipShot::Init(Player* player, int combo) {
#ifdef _PRINT_PLAYER_ATTACK
	std::string s = "attack hip-shot combo :" + std::to_string(combo);
	DebugPrint_WATA(s.c_str());
#endif
	m_isDone = false;
	//m_isContinuAttack = false;
	player->PlayAnimation(Player::EnAnimation::enAttack);

	//auto& enemyManager = EnemyManager::GetEnemyManager();
	//enemyManager.ApplyAoeDamage(/*attack origin*/ player->GetPosition(), m_range, m_damageAmount * combo);

}

void PlayerAttackHipShot::Execute(Player* player) {

	const auto delta = gameTime()->GetDeltaTime();
	auto lxf = g_pad[0]->GetLStickXF();
	Quaternion rot = Quaternion::Identity;
	const float rotationSpeed = 100.f;
	rot.SetRotationDegY(lxf * rotationSpeed * delta);
	auto prot = player->GetRotation();
	prot.Multiply(rot);
	player->SetRotation(prot);

	if (g_pad[0]->IsPress(EnButton::enButtonB)) {
		GenerateProjectiles(player);
		Move(player);
	}
	else {
		m_isDone = true;
	}

}

void PlayerAttackHipShot::GenerateProjectiles(Player* player) {
	m_shotIntervalTimer += gameTime()->GetDeltaTime();
	if (m_shotIntervalTimer >= m_shotInterval) {

		auto projectileL = NewGO<Projectile>(0, true);
		auto projectileR = NewGO<Projectile>(0, true);

		auto vel = player->GetForward();
		vel.y = 0.f;

		//
		const float defSpeed = 400.f;
		const float lifeSpan = 10.f;
		const float range = 10.f;
		const float scale = 1.f;

		auto posLeft = player->GetGun()->GetGunModelBone(Gun::GunBone::Left)->GetWorldMatrix().GetTransrate();
		auto posRight = player->GetGun()->GetGunModelBone(Gun::GunBone::Right)->GetWorldMatrix().GetTransrate();

		projectileL->Init(posLeft, scale, vel, defSpeed, lifeSpan, range);
		projectileR->Init(posRight, scale, vel, defSpeed, lifeSpan, range);

		//projectile->Init(player->GetPosition(), vel);

		//Effect
		player->GetGun()->PlayFireEffects();

		m_shotIntervalTimer = 0.f;
	}
}

void PlayerAttackHipShot::Move(Player* player) {

}