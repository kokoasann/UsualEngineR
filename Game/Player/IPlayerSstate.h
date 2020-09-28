#pragma once
class Player;

/// <summary>
/// 
/// </summary>
class IPlayerState
{
public:
	IPlayerState() {};
	virtual ~IPlayerState() {};
	virtual void Enter(Player*) = 0;
	virtual IPlayerState* Update(Player*) = 0;
	virtual void Exit(Player*) = 0;
};