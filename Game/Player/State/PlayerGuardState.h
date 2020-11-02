#pragma once
#include"IPlayerSstate.h"
class Player;

class PlayerGuardState : public IPlayerState
{
public:
	PlayerGuardState();
	~PlayerGuardState() override;

	void Enter(Player*) override;
	IPlayerState* Update(Player*) override;
	void Exit(Player*) override;

private:
	const float m_guardParam = 40.f; //何パーセントダメージ軽減する?
};

