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
	float Approach(float goal, float current, float dt);
	Vector3 Approach(Vector3 goal, Vector3 current, float dt);
	Vector3 m_velocity = Vector3::Zero;
	Vector3 m_vecVelocityGoal = Vector3::Zero;
};