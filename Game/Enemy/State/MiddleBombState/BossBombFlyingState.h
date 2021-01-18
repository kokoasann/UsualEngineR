#pragma once
#include "Enemy/State/IEnemyState.h"

class BossBombFlyingState final : public IEnemyState
{
public:
	BossBombFlyingState();
	~BossBombFlyingState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
	void  OnAttacked(IEnemy* e);
private:
};
