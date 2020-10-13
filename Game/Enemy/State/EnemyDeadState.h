#pragma once
#include "IEnemyState.h"

class EnemyDeadState : public IEnemyState
{
public:
	EnemyDeadState();
	~EnemyDeadState();
	virtual void Enter(IEnemy* e) override;
	virtual IEnemyState* Update(IEnemy* e) override;
	virtual void Exit(IEnemy* e) override;
};

