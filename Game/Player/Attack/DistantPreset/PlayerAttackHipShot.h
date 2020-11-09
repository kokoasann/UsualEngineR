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
		return m_isContinuAttack;
	}
private:
	bool m_isDone = false;
	bool m_isContinuAttack = false;
	const float m_interval = 0.5f;
	float m_timer = 0.f;

	float m_range = 50.f;
	float m_damageAmount = 15.f;
};

