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
	const float m_VELOCITY_MAX = 20.f;
	const float m_GRAVITY = -30.f;
	float m_QUICKNESS = 50.f;
	const float m_RUN_SPEED_PARAM = 2.0f;
	Vector3 m_velocity = Vector3::Zero;
	Vector3 m_vecVelocityGoal = Vector3::Zero;
};