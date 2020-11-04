#include "stdafx.h"
#include "PlayerDeadState.h"
#include "../Player.h"

PlayerDeadState::PlayerDeadState() {
}

PlayerDeadState::~PlayerDeadState() {

}

void PlayerDeadState::Enter(Player* p) {
#ifdef _PRINT_PLAYER_STATE
	DebugPrint_WATA("Player - Enter Dead State\n");
#endif
	auto& vel = p->GetLocalVelocity();
	p->SetVelocity(Vector3::Zero);
	p->PlayAnimation(Player::EnAnimation::enDead);
}

void PlayerDeadState::Exit(Player* p) {
#ifdef _PRINT_PLAYER_STATE
	DebugPrint_WATA("Player - Exit Dead State\n");
#endif
}

IPlayerState* PlayerDeadState::Update(Player* p) {
	return this;
}