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
	//Move
	auto lxf = g_pad[0]->GetLStickXF();
	auto lyf = g_pad[0]->GetLStickYF();

	m_vecVelocityGoal.x = lxf;
	m_vecVelocityGoal.z = lyf;


	if (g_pad[0]->IsTrigger(EnButton::enButtonA)) {
		 auto nextState =  p->GetState(Player::EnState::enFlying);
		return nextState;
	}
	return this;
}

void PlayerGroundState::Exit(Player* p) {
	printf("Ground Exit\n");
}


