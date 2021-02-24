#pragma once
#include "IPlayerSstate.h"

class PlayerDeadState : public IPlayerState
{
public:
	PlayerDeadState();
	~PlayerDeadState();
	void Enter(Player*) override;
	IPlayerState* Update(Player*) override;
	void Exit(Player*) override;
private:
	const float m_RespawnTime = 5.f;
	const float m_fadeOutTime = 3.0f;
	float m_respawnTimer = 0.f;
	bool m_isFadedToRespawn = false;
	float m_gravity = 9.f;

	Vector3 m_velocity = Vector3::Zero;
	Vector3 m_fricVel = Vector3::Zero;
};

