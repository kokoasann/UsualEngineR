#include "stdafx.h"
#include "PlayerLongRangeAttack.h"
#include "../Player.h"
#include "../Pod/Pod.h"
#include "Projectile.h"
#include "../../Enemy/IEnemy.h"
#include "../../GameManager.h"
#include "../../Camera/GameCamera.h"
#include "../../Enemy/EnemyManager.h"

PlayerLongRangeAttack::PlayerLongRangeAttack() {
}

PlayerLongRangeAttack::~PlayerLongRangeAttack() {

}

void PlayerLongRangeAttack::Execute(Player* p) {
	auto delta = gameTime()->GetDeltaTime();
	m_shotIntervalTimer += delta;
	if (m_shotIntervalTimer >= m_interval) {
		DebugPrint_WATA("distant attacking\n");
		auto projectile = NewGO<Projectile>(0);
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
		projectile->Init(p->GetPod()->GetPosition(), vel);
		m_shotIntervalTimer = 0.f;
	}
}
