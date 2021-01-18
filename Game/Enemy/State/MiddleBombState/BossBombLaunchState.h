#pragma once
#include "Enemy/State/IEnemyState.h"

class BossBombLaunchState final : public IEnemyState
{
public:
	BossBombLaunchState();
	~BossBombLaunchState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
	void  OnAttacked(IEnemy* e);
private:
};
