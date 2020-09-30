#include "stdafx.h"
#include "PlayerFlyingState.h"
#include "Player.h"

PlayerFlyingState::PlayerFlyingState()
{

}

PlayerFlyingState::~PlayerFlyingState()
{

}

void PlayerFlyingState::Enter(Player* p) {
	printf("fly enter\n");
}

IPlayerState*  PlayerFlyingState::Update(Player* p) {
	if (g_pad[0]->IsTrigger(EnButton::enButtonA)) {
		auto nextState = p->GetState(Player::EnState::enGround);
		return nextState;
	}
	return this;
}

void PlayerFlyingState::Exit(Player* p) {
	printf("Fly Exit\n");
}