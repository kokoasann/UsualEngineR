#include "stdafx.h"
#include "PlayerLongRangeAttack.h"
#include "../Player.h"
#include "Projectile.h"

PlayerLongRangeAttack::PlayerLongRangeAttack() {
}

PlayerLongRangeAttack::~PlayerLongRangeAttack() {

}

void PlayerLongRangeAttack::Execute(Player* p) {
	m_intervalTimer += gameTime()->GetDeltaTime();
	if (m_intervalTimer >= m_interval) {
		printf("distant attacking\n");
		auto projectile = NewGO<Projectile>(0);
		projectile->Init(p->GetPosition(),p->GetForward());
		m_intervalTimer = 0.f;
	}
}
