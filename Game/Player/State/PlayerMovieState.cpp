#include "stdafx.h"
#include "PlayerMovieState.h"
#include "Player/Player.h"

PlayerMovieState::PlayerMovieState()
{

}

PlayerMovieState::~PlayerMovieState()
{

}

void PlayerMovieState::Enter(Player* p) {
#ifdef _PRINT_PLAYER_STATE
	DebugPrint_WATA("Player : Enter Movie State\n");
#endif
	p->StopThrusters();
	p->SetLocalVelocity(Vector3::Zero);
	p->SetVelocity(Vector3::Zero);
	p->SetExternalVelocity(Vector3::Zero);
}


IPlayerState* PlayerMovieState::Update(Player* p) {
	return this;
}


void PlayerMovieState::Exit(Player* p) {
#ifdef _PRINT_PLAYER_STATE
	DebugPrint_WATA("Player : Exit Movie State\n");
#endif
}