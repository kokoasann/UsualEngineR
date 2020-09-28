#include "stdafx.h"
#include "PlayerGroundState.h"

PlayerGroundState::PlayerGroundState()
{

}

PlayerGroundState::~PlayerGroundState()
{

}

void PlayerGroundState::Enter(Player* p){
	printf("Ground Enter\n");
}

IPlayerState* PlayerGroundState::Update(Player* p) {
	return this;
}

void PlayerGroundState::Exit(Player* p) {
	printf("Ground Exit\n");
}


