#pragma once
#include "Enemy/State/IEnemyState.h"

class EnemyShortBigPunch final : public IEnemyState
{
public:
	EnemyShortBigPunch();
	~EnemyShortBigPunch();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
	void  OnAttacked(IEnemy* e);
private:
};
