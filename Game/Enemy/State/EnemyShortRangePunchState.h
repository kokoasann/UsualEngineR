#pragma once
#include "IEnemyState.h"

class EnemyShortRangePunchState final : public IEnemyState
{
public:
	EnemyShortRangePunchState();
	~EnemyShortRangePunchState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
private:
};
