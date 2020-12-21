#pragma once
#include "Enemy/State/IEnemyState.h"

class EnemyLongBigBlownState final : public IEnemyState
{
public:
	EnemyLongBigBlownState();
	~EnemyLongBigBlownState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
	void  OnAttacked(IEnemy* e);
private:
};
