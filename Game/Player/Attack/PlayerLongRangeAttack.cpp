#include "stdafx.h"
#include "PlayerLongRangeAttack.h"
#include "../Player.h"
#include "../Pod/Pod.h"
#include "Projectile.h"

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
		auto vel = p->GetForward() + p->GetLocalVelocity() * delta;
		projectile->Init(p->GetPod()->GetPosition(), vel);
		m_shotIntervalTimer = 0.f;
	}
}
