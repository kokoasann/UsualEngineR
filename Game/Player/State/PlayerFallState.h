#pragma once
#include"IPlayerSstate.h"
#include "../Player.h"

class Player;
/// <summary>
/// 
/// </summary>
class PlayerFallState :public IPlayerState
{
public:
	PlayerFallState();
	~PlayerFallState() override;
	void Enter(Player*) override;
	IPlayerState* Update(Player*) override;
	void Exit(Player*) override;

private:

	//const float m_GRAVITY = -15.f;
	const float m_GRAVITY = -10.f;
	Vector3 m_velocity = Vector3::Zero;
	const float m_VELOCITY_MAX = 4.f;
	Vector3 m_velocityGoal = Vector3::Zero;
	const float m_QUICKNESS = 70.f;
	const float m_ENDURANCE_AUTO_CHARGE_AMOUNT = 5.f;

};