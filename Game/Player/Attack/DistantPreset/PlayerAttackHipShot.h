#pragma once
#include "../IPlayerAttack.h"

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
	//attack
	bool m_isDone = false;
	const float m_interval = 0.5f;

	//shot
	const float m_shotInterval = 0.2f;
	float m_shotIntervalTimer = m_shotInterval;

	float m_range = 50.f;
	const float m_damageAmount = 15.f;
};

