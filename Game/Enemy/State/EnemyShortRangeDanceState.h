#pragma once
#include "IEnemyState.h"

class EnemyShortRangeDanceState final : public IEnemyState
{
public:
	EnemyShortRangeDanceState();
	~EnemyShortRangeDanceState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
private:
};
