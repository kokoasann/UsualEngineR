#include "stdafx.h"
#include "PlayerAttackHipShot.h"
#include "../../Player.h"
#include "../../../Enemy/EnemyManager.h"
#include "../Projectile.h"

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
		m_shotIntervalTimer += gameTime()->GetDeltaTime();
		if (m_shotIntervalTimer >= m_shotInterval) {
			DebugPrint_WATA("hip shot\n");
			auto projectile = NewGO<Projectile>(0);
			auto vel = player->GetForward();
			vel.y = g_camera3D->GetForward().y;
			projectile->Init(player->GetPosition(), vel);
			m_shotIntervalTimer = 0.f;
		}
	}
	else {
		m_isDone = true;
	}

}


