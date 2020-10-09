#pragma once
#include "IPlayerAttack.h"

class PlayerAttackSlash : public IPlayerAttack
{
public:
	PlayerAttackSlash();
	virtual ~PlayerAttackSlash() override;

	virtual void Init(int combo) override;;
	virtual void Execute() override;

	virtual const bool IsDone() override{
		return m_isDone;
	}

	virtual const float GetInterval() override{
		return m_interval;
	}
	virtual const bool IsContinueAttack() override{
		return m_isContinuAttack;
	}
private:
	bool m_isDone = false;
	bool m_isContinuAttack = false;
	const float m_interval = 0.5f;
};

