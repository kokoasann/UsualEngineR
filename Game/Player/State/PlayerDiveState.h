#pragma once
#include "IPlayerSstate.h"
/// <summary>
/// 
/// </summary>
class PlayerDiveState :public IPlayerState
{
public:
	PlayerDiveState();
	virtual ~PlayerDiveState() override;
	void Enter(Player*) override;
	IPlayerState* Update(Player*) override;
	void Exit(Player*) override;

private:

	const float m_DIVE_STAMINA_COST = 10.f;
	const float m_VELOCITY_MAX = 10.f;
	const float m_GRAVITY = -9.81f;
	const float m_QUICKNESS = 50.f;
	const float m_FIRST_VEL_PARAM = 15.f;

	Vector3 m_velocity = Vector3::Zero;
	Vector3 m_velocityGoal = Vector3::Zero;
	bool m_isCanDive = true;
};
