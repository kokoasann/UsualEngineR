#pragma once
#include "IPlayerSstate.h"
class IPlayerAttack;
//for normal attack on ground

class PlayerAttackState : public IPlayerState
{
public:
	PlayerAttackState();
	~PlayerAttackState();
	void Enter(Player*) override;
	IPlayerState* Update(Player*) override;
	void Exit(Player*) override;
private:
	IPlayerAttack* m_currentAttack = nullptr;
	int m_combo = 1;
	const int m_MAX_COMBO = 3;
	float m_elpased = 0.f;
};

