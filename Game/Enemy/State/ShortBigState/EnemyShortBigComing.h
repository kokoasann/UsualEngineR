#pragma once
#include "Enemy/State/IEnemyState.h"

class EnemyShortBigComing final : public IEnemyState
{
public:
	EnemyShortBigComing();
	~EnemyShortBigComing();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
	void  OnAttacked(IEnemy* e);
private:
};
