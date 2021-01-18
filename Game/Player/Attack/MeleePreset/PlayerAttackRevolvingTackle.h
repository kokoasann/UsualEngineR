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
	float m_REVOLVE_TIME = 1.f;

	const float m_tacklePower = 150.f;
	float m_range = 15.f;
	const float m_damageAmount = m_DEFAULT_ATTACK_DAMAGE * 7.f;

	const float m_StaminaCost = 30.f;
	const float m_BoostCost = 30.f;

	const float m_knockBackPower = 230.f;

	bool m_canDoAttack = true;

};


