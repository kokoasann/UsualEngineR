#pragma once
#include "../IPlayerAttack.h"

class PlayerAttackThrowPod : public IPlayerAttack
{
public:
	PlayerAttackThrowPod();
	virtual ~PlayerAttackThrowPod() override;

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
	bool m_isDone = false;
	const float m_interval = 0.1f;
	float m_timer = 0.f;

	const float m_throwPower = 200.f;
	const float m_StaminaCost = 30.f;
	const float m_BoostCost = 30.f;

	bool m_canDoAttack = true;

	float m_range = 50.f;
	const float m_damageAmount = m_DEFAULT_ATTACK_DAMAGE * 8.f;

	bool threwFlag = false;
};

