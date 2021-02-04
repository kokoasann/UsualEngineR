#pragma once
#include "IPlayerSstate.h"
/// <summary>
/// 
/// </summary>
class PlayerStunState :public IPlayerState
{
public:
	PlayerStunState();
	virtual ~PlayerStunState() override;
	void Enter(Player*) override;
	IPlayerState* Update(Player*) override;
	void Exit(Player*) override;

private:
	float m_timer = 0.f;
	const float m_stunTime = 1.f;

	const float m_VELOCITY_MAX = 10.f;
	//const float m_GRAVITY = -9.81f;
	const float m_GRAVITY = 500.f;
	const float m_QUICKNESS = 300.f;
	const float m_FIRST_VEL_PARAM = 15.f;

	Vector3 m_velocity = Vector3::Zero;
	Vector3 m_velocityGoal = Vector3::Zero;

	Vector3 m_velFriction = Vector3::Zero;
	const float m_friction = 200.f;
};
