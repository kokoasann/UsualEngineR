#pragma once
#include "Enemy/State/IEnemyState.h"

class BossBombRollingState final : public IEnemyState
{
public:
	BossBombRollingState();
	~BossBombRollingState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
	void  OnAttacked(IEnemy* e);
private:
};
