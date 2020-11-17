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

};

