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
	m_intervalTimer += gameTime()->GetDeltaTime();
	if (m_intervalTimer >= m_interval) {
		DebugPrint_WATA("distant attacking\n");
		auto projectile = NewGO<Projectile>(0);
		projectile->Init(p->GetPod()->GetPosition(),p->GetForward());
		m_intervalTimer = 0.f;
	}
}
