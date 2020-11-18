#pragma once
#include "../IEnemyState.h"

class EnemyMeleeFlyState : public IEnemyState
{
public:
	EnemyMeleeFlyState();
	virtual ~EnemyMeleeFlyState() override;
	virtual void Enter(IEnemy* e) override;
	virtual IEnemyState* Update(IEnemy* e) override;
	virtual void Exit(IEnemy* e) override;
private:

};