#pragma once
#include "../IEnemyState.h"

class EnemyMeleePunchState : public IEnemyState
{
public:
	EnemyMeleePunchState();
	virtual ~EnemyMeleePunchState() override;
	virtual void Enter(IEnemy* e) override;
	virtual IEnemyState* Update(IEnemy* e) override;
	virtual void Exit(IEnemy* e) override;
private:

};

