#pragma once
#include"IPlayerSstate.h"
#include "../Player.h"
class Player;
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

	void CameraWorldMove(Player*);
	void TargettingEnemyMove(Player*);

	//Auto charge
	const float m_BOOST_AUTO_CHARGE_AMOUNT = 5.f;
	const float m_ENDURANCE_AUTO_CHARGE_AMOUNT = 5.f;

	const float m_RUN_COST = 20.f;
	const float m_VELOCITY_MAX = 15.f;
	const float m_SIDE_MOVE_VELOCITY_MAX = 9.f;
	const float m_GRAVITY = -30.f;
	float m_QUICKNESS = 50.f;
	const float m_RUN_SPEED_PARAM = 2.0f;
	Vector3 m_velocity = Vector3::Zero;
	Vector3 m_vecVelocityGoal = Vector3::Zero;
};