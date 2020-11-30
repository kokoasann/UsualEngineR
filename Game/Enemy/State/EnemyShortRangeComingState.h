#pragma once
#include "IEnemyState.h"

class EnemyShortRangeComingState final : public IEnemyState
{
public:
	EnemyShortRangeComingState();
	~EnemyShortRangeComingState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
private:
};
