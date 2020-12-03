#pragma once
#include "IEnemyState.h"

class EnemyShortRangeIdleState final : public IEnemyState
{
public:
	EnemyShortRangeIdleState();
	~EnemyShortRangeIdleState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
private:
};