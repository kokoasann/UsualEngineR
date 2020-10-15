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
	const float m_DIVE_STAMINA_COST = 50.f;
	const float m_VELOCITY_MAX = 100.f;
	const float m_GRAVITY = -9.81f;
	float m_QUICKNESS = 500.f;
	const float m_RUN_SPEED_PARAM = 2.0f;
	Vector3 m_velocity = Vector3::Zero;
	Vector3 m_velocityGoal = Vector3::Zero;
	bool m_isCanDive = true;
};
