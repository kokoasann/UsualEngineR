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
	if (g_pad[0]->IsTrigger(EnButton::enButtonA)) {
		 auto nextState =  p->GetState(Player::EnState::enFlying);
		return nextState;
	}
	return this;
}

void PlayerGroundState::Exit(Player* p) {
	printf("Ground Exit\n");
}


