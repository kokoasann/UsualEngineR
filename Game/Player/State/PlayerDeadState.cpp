#include "stdafx.h"
#include "PlayerDeadState.h"
#include "../Player.h"

PlayerDeadState::PlayerDeadState() {
}

PlayerDeadState::~PlayerDeadState() {

}

void PlayerDeadState::Enter(Player* p) {
	printf("Player - Enter Dead State\n");
	auto& vel = p->GetLocalVelocity();
	p->SetVelocity(Vector3::Zero);
	p->PlayAnimation(Player::EnAnimation::enDead);
}

void PlayerDeadState::Exit(Player* p) {
	printf("Player - Exit Dead State\n");
}

IPlayerState* PlayerDeadState::Update(Player* p) {
	return this;
}