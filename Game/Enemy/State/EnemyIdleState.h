#pragma once
#include "IEnemyState.h"

class EnemyIdleState : public IEnemyState
{
public:
	EnemyIdleState();
	virtual ~EnemyIdleState() override;
	virtual void Enter(IEnemy* e) override;
	virtual IEnemyState* Update(IEnemy* e) override;
	virtual void Exit(IEnemy* e) override;
private:

};

