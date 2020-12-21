#pragma once
#include "Enemy/State/IEnemyState.h"

class EnemyShortBigBlown final : public IEnemyState
{
public:
	EnemyShortBigBlown();
	~EnemyShortBigBlown();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
	void  OnAttacked(IEnemy* e);
private:
};
