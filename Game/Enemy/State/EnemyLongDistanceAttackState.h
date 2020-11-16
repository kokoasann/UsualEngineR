#pragma once
#include "Enemy/State/IEnemyState.h"

class EnemyLongDistanceAttackState final :public IEnemyState
{
public:
	

	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
private:

	
};