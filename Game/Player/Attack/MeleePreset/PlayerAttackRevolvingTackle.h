#pragma once
#include "../IPlayerAttack.h"

class PlayerAttackRevolvingTackle : public IPlayerAttack
{
public:
	PlayerAttackRevolvingTackle();
	virtual ~PlayerAttackRevolvingTackle() override;

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
	const float m_interval = 0.3f;
	float m_timer = 0.f;

	const float m_tacklePower = 500.f;
	float m_range = 30.f;
	float m_damageAmount = 100.f;

};


