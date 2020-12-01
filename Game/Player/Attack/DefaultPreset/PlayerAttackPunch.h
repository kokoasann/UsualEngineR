#pragma once
#include "../IPlayerAttack.h"

class PlayerAttackPunch : public IPlayerAttack
{
public:
	PlayerAttackPunch();
	virtual ~PlayerAttackPunch() override;

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
	const float m_interval = 0.1f;
	float m_timer = 0.f;

	int m_combo = 0;
	float m_range = 50.f;
	bool m_hasAlreadyAttacked = false;
	const float m_damageAmount = m_DEFAULT_ATTACK_DAMAGE;
};

