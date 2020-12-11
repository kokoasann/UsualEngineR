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
	bool m_canRise = true;
	bool m_isFirstRising = true;

	Vector3 m_velocity = Vector3::Zero;
	Vector3 m_velocityGoal = Vector3::Zero;
	Vector3 m_bPos = Vector3::Zero;

	const float m_ENDURANCE_AUTO_CHARGE_AMOUNT = 5.f;

	const float m_rise = 3.5f;
	const float m_MIN_ALTITUDE = 50.f;
	const float m_VELOCITY_BOOST = 2.f;
	const float m_VELOCITY_MAX = 4.f;
	//const float m_QUICKNESS = 40.f;
	const float m_QUICKNESS = 70.f;
	const float m_AUTO_RISE_PARAM = 10.f; //©“®ã¸‚ÌÅ‘åVelocity‚Í’Êí‚Ì‰½”{‚©

	const float m_FIRST_RISE_BOOST_COST = 50.f;
	const float m_BOOST_EFFICIENCY = 300.f / 15.f; //”R”ï,’l‚ª‘å‚«‚¢‚Ù‚Çˆ«‚¢...
	//const float m_BOOST_EFFICIENCY = 0.f; //”R”ï,’l‚ª‘å‚«‚¢‚Ù‚Çˆ«‚¢...
	const float m_BOOST_RUN_EFFICIENCY = 300.f / 5.f;
	//const float m_BOOST_RUN_EFFICIENCY = 0.f;
	const float m_RISE_BOOST_EFFICIENCY = 300.f / 1.5f;
	//const float m_RISE_BOOST_EFFICIENCY = 0.f;
};