#pragma once
#include "Enemy/State/IEnemyState.h"

class EnemyShortBigStrongPunch final : public IEnemyState
{
public:
	EnemyShortBigStrongPunch();
	~EnemyShortBigStrongPunch();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
	void  OnAttacked(IEnemy* e);
private:
};
