#pragma once
#include "IPlayerSstate.h"
class Player;

/// <summary>
/// 
/// </summary>
class PlayerFlyingState :public IPlayerState
{
public:
	PlayerFlyingState();
	virtual ~PlayerFlyingState() override;
	void Enter(Player*) override;
	IPlayerState* Update(Player*) override;
	void Exit(Player*) override;
private:

};