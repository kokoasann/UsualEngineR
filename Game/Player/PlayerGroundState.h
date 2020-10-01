#pragma once
#include"IPlayerSstate.h"
#include "Player.h"

/// <summary>
/// 
/// </summary>
class PlayerGroundState :public IPlayerState
{
public:
	PlayerGroundState();
	~PlayerGroundState() override;
	void Enter(Player*) override;
	IPlayerState* Update(Player*) override;
	void Exit(Player*) override;

private:
	const float m_MAX_VELOCITY = 20.f;
	const float m_QUICKNESS = 40.f;
	Vector3 m_velocity = Vector3::Zero;
	Vector3 m_vecVelocityGoal = Vector3::Zero;
};