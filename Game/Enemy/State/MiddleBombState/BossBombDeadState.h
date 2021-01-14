#pragma once
#include "Enemy/State/IEnemyState.h"

class BossBombDeadState final : public IEnemyState
{
public:
	BossBombDeadState();
	~BossBombDeadState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
	void  OnAttacked(IEnemy* e);
private:
};
