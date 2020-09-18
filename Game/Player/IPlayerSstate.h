#pragma once

/// <summary>
/// 
/// </summary>
class IPlayerSstate
{
public:
	virtual ~IPlayerSstate();
	virtual void Enter() = 0;
	virtual IPlayerSstate* Update() = 0;
	virtual void Exit() = 0;
};