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

	const float m_GRAVITY = -15.f;
	Vector3 m_velocity = Vector3::Zero;

	bool m_hasEnoughStaminaToRun = true;
};