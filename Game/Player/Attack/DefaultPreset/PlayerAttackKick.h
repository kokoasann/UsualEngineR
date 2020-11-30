#pragma once
#include "../IPlayerAttack.h"

class PlayerAttackKick : public IPlayerAttack
{
public:
	PlayerAttackKick();
	virtual ~PlayerAttackKick() override;

	virtual void Init(Player*, int combo) override;;
	virtual void Execute(Player*) override;

	virtual const bool IsDone() override {
		return m_isDone;
	}

	virtual const float GetInterval() override {
		return m_interval;
	}
	virtual const bool IsContinueAttack() override {
		//ÉRÉìÉ{Ç»ÇµÅB
		return false;
	}
private:
	bool m_isDone = false;
	const float m_interval = 0.1f;
	float m_timer = 0.f;

	float m_range = 50.f;
	const float m_damageAmount = m_DEFAULT_ATTACK_DAMAGE * 5.f;
};

