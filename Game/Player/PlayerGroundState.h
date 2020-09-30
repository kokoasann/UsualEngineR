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
	Vector3 m_position = Vector3::Zero;
	Vector3 m_vecVelocityGoal = Vector3::Zero;

	float m_velocity = 0.f;
	float m_maxSpeed = 10.f;
	float m_currentSpeed = 0.f;

};