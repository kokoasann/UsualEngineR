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
	const float m_RespawnTime = 1.f;
	float m_respawnTimer = 0.f;
};

