#pragma once
#include "IEnemyState.h"

class EnemyShortRangeLongJampAttackState final : public IEnemyState
{
public:
	EnemyShortRangeLongJampAttackState();
	~EnemyShortRangeLongJampAttackState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
private:
};
