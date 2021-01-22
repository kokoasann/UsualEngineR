#pragma once
#include "Enemy/State/IEnemyState.h"

class BossBombGuardState final : public IEnemyState
{
public:
	BossBombGuardState();
	~BossBombGuardState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
	void  OnAttacked(IEnemy* e);
private:
	float m_timer = 0.;
	float m_timeLimit = 2.;
};
