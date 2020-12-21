#pragma once
#include "Enemy/State/IEnemyState.h"

class EnemyShortBigStun final : public IEnemyState
{
public:
	EnemyShortBigStun();
	~EnemyShortBigStun();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
	void  OnAttacked(IEnemy* e);
private:
};
