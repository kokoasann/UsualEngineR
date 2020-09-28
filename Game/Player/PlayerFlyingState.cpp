#include "stdafx.h"
#include "PlayerFlyingState.h"

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
	return this;
}

void PlayerFlyingState::Exit(Player* p) {
	printf("Fly Exit\n");
}