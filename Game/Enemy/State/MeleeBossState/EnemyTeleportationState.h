#pragma once
#include "../IEnemyState.h"

class EnemyTeleportationState : public IEnemyState
{
public:
	EnemyTeleportationState();
	virtual ~EnemyTeleportationState() override;
	virtual void Enter(IEnemy* e) override;
	virtual IEnemyState* Update(IEnemy* e) override;
	virtual void Exit(IEnemy* e) override;
private:

};