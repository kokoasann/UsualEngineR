#pragma once
#include "IEnemyState.h"

class EnemyShortRangeStrongPunchState final : public IEnemyState
{
public:
	EnemyShortRangeStrongPunchState();
	~EnemyShortRangeStrongPunchState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
private:
};
