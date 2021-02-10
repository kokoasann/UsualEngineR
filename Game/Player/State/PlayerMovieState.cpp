#include "stdafx.h"
#include "PlayerMovieState.h"

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
}


IPlayerState* PlayerMovieState::Update(Player* p) {
	return this;
}


void PlayerMovieState::Exit(Player* p) {
#ifdef _PRINT_PLAYER_STATE
	DebugPrint_WATA("Player : Exit Movie State\n");
#endif
}