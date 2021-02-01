#pragma once
#include"IPlayerSstate.h"
#include "../Player.h"
#include "Player/Attack/DistantPreset/PlayerAttackHipShot2.h"

class Player;
class KeyHelp;

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
	const float m_DefaultAnimSpeed = 1.f;

	void CameraWorldMove(Player*);
	void TargettingEnemyMove(Player*);

	//Auto charge
	const float m_BOOST_AUTO_CHARGE_AMOUNT = 300.f / 15.f;
	const float m_ENDURANCE_AUTO_CHARGE_AMOUNT = 300.f / 15.f;

	const float m_RUN_COST = 300.f / 5.f;
	const float m_VELOCITY_MAX = 2.f;
	const float m_SIDE_MOVE_VELOCITY_MAX = 1.5f;
	const float m_GRAVITY = -15.f;
	float m_QUICKNESS = 50.f;
	const float m_RUN_SPEED_PARAM = 1.5f;
	const float m_RUN_BEGIN_COST = 10.f;
	Vector3 m_velocity = Vector3::Zero;
	Vector3 m_vecVelocityGoal = Vector3::Zero;

	bool m_hasEnoughStaminaToRun = true;

	PlayerAttackHipShot2 m_hipShot;

	KeyHelp* m_keyHelp_Run = nullptr;
};