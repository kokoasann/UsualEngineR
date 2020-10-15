#pragma once
#include "IEnemyState.h"

class EnemyBattleState final : public IEnemyState
{
public:
	EnemyBattleState();
	~EnemyBattleState();
	virtual void Enter(IEnemy* e) override;
	virtual IEnemyState* Update(IEnemy* e) override;
	virtual void Exit(IEnemy* e) override;
};
