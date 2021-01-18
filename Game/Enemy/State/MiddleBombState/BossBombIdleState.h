#pragma once
#include "Enemy/State/IEnemyState.h"

class BossBombIdleState final : public IEnemyState
{
public:
	BossBombIdleState();
	~BossBombIdleState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
	void  OnAttacked(IEnemy* e);
private:
};
