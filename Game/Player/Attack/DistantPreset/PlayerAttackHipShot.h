#pragma once
#include "../IPlayerAttack.h"
//Note : This class is no longer used in this project.

class PlayerAttackHipShot : public IPlayerAttack
{
public:
	PlayerAttackHipShot();
	virtual ~PlayerAttackHipShot() override;

	virtual void Init(Player*, int combo) override;;
	virtual void Execute(Player*) override;

	virtual const bool IsDone() override {
		return m_isDone;
	}

	virtual const float GetInterval() override {
		return m_interval;
	}

	virtual const bool IsContinueAttack() override {
		return false;
	}

private:
	void GenerateProjectiles(Player* player);
	void Move(Player* player);

	//attack
	bool m_isDone = false;
	const float m_interval = 0.5f;

	//shot
	const float m_shotInterval = 0.2f;
	float m_shotIntervalTimer = m_shotInterval;

	float m_range = 50.f;
	const float m_damageAmount = m_DEFAULT_ATTACK_DAMAGE * 1.f / 2.f;
};

