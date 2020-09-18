#pragma once
#include "IPlayerSstate.h"

/// <summary>
/// 
/// </summary>
class PlayerGroundState :public IPlayerSstate
{
public:
	PlayerGroundState();
	~PlayerGroundState();
	void Enter() override;
	IPlayerSstate* Update() override;
	void Exit() override;
private:

};