#pragma once
#include "IEnemyState.h"

class EnemySlashState : public IEnemyState
{
public:
	EnemySlashState();
	~EnemySlashState();
	virtual void Enter(IEnemy* e) override;
	virtual IEnemyState* Update(IEnemy* e) override;
	virtual void Exit(IEnemy* e) override;
private:
	float m_timer = 0.f;
	const float m_damageAmount = 10.f;
	const float m_range = 30.f;
	const float m_intervalSec = 1.f;
};

