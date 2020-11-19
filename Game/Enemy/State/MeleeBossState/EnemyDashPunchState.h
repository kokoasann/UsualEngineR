#pragma once
#include "../IEnemyState.h"

class EnemyDashPunchState : public IEnemyState
{
public:
	EnemyDashPunchState();
	virtual ~EnemyDashPunchState() override;
	virtual void Enter(IEnemy* e) override;
	virtual IEnemyState* Update(IEnemy* e) override;
	virtual void Exit(IEnemy* e) override;
private:
	float m_timer = 0.f;
	const float m_damageAmount = 15.f;
	const float m_range = 30.f;
	const float m_intervalSec = 0.5f;
};

