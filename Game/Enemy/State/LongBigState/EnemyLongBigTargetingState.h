#pragma once
#include "Enemy/State/IEnemyState.h"

class EnemyLongBigTargetingState final : public IEnemyState
{
public:
	EnemyLongBigTargetingState();
	~EnemyLongBigTargetingState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
	void  OnAttacked(IEnemy* e);
private:
};
