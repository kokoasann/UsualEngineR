#pragma once
#include "IPlayerSstate.h"

/// <summary>
/// 
/// </summary>
class PlayerMovieState :public IPlayerState
{
public:
	PlayerMovieState();
	virtual ~PlayerMovieState();
	void Enter(Player*) override;
	IPlayerState* Update(Player*) override;
	void Exit(Player*) override;
private:

};
