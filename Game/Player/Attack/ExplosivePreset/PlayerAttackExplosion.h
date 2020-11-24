#pragma once
#include "../IPlayerAttack.h"

class PlayerAttackExplosion : public IPlayerAttack
{
public:
	PlayerAttackExplosion();
	virtual ~PlayerAttackExplosion() override;

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

	bool m_isBombed = false;

	float m_range = 50.f;
	float m_damageAmount = 15.f;
	const float m_tacklePower = 500.f;
	const float m_ExplodeDamageRange = 100.f;
	const float m_explodeDamage = 200.f;
};



