#pragma once
#include "../IPlayerAttack.h"

class PlayerAttackHipShot2
{
public:
	PlayerAttackHipShot2();
	~PlayerAttackHipShot2();

	void GenerateProjectiles(Player* player);

private:
	//shot
	const float m_shotInterval = 0.2f;
	float m_shotIntervalTimer = m_shotInterval;

	float m_range = 50.f;
	const float m_damageAmount = 10.f * 1.f / 2.f;
};

