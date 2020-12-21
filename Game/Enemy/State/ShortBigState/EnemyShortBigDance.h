#pragma once
#include "Enemy/State/IEnemyState.h"

class EnemyShortBigDance final : public IEnemyState
{
public:
	EnemyShortBigDance();
	~EnemyShortBigDance();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
	void  OnAttacked(IEnemy* e);
private:
};
