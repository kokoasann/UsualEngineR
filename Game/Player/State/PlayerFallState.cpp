#include "stdafx.h"
#include "PlayerFallState.h"
#include "../../Camera/GameCamera.h"
#include <cmath>

PlayerFallState::PlayerFallState()
{
}

PlayerFallState::~PlayerFallState()
{
}

void PlayerFallState::Enter(Player* p){

	p->StopThrusters();

	m_velocity = p->GetLocalVelocity();
	p->PlayAnimation(Player::EnAnimation::enIdle, m_AnimInterpolate);
#ifdef _PRINT_PLAYER_STATE
	DebugPrint_WATA("Player Enter Fall State\n");
#endif
}

IPlayerState* PlayerFallState::Update(Player* p) {

	if (p->IsOnGround()) {
		auto nextState = p->GetState(Player::EnState::enGround);
		return nextState;
	}

	if (g_pad[0]->IsTrigger(EnButton::enButtonA)) {
		auto nextState = p->GetState(Player::EnState::enFlying);
		return nextState;
	}

	auto vel = Vector3::Zero;
	vel.y += m_GRAVITY;

	vel *= p->GetSpeed();
	p->SetVelocity(vel);

	return this;
}

void PlayerFallState::Exit(Player* p) {
#ifdef _PRINT_PLAYER_STATE
	DebugPrint_WATA("Exit FallState\n");
#endif
}
