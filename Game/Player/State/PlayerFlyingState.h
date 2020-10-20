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
	Vector3 m_velocity = Vector3::Zero;
	Vector3 m_velocityGoal = Vector3::Zero;
	Vector3 m_bPos = Vector3::Zero;

	const float m_rise = 10.0f;
	const float m_VELOCITY_BOOST = 2.f;
	const float m_VELOCITY_MAX = 40.f;
	const float m_QUICKNESS = 40.f;
	const float m_AUTO_RISE_QUICKNESS = 10.f; 

	const float m_BOOST_EFFICIENCY = 50.f; //燃費
	const float m_ACCELERATE_PARAM = 1.5f; //加速時の燃費は通常時の何倍にするか
};