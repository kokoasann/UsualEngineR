#pragma once
#include "Enemy/State/IEnemyState.h"

class EnemyLongDistanceAttackState final :public IEnemyState
{
public:
	

	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
private:
	float m_timer = 0.f;
	float m_timeSpan = 2.f;
};