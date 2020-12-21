#pragma once
#include "Enemy/State/IEnemyState.h"

class EnemyLongBigAttackState final : public IEnemyState
{
public:
	EnemyLongBigAttackState();
	~EnemyLongBigAttackState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
	void  OnAttacked(IEnemy* e);
private:
};
