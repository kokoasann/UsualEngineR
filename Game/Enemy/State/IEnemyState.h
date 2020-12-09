#pragma once
#include "GameManager.h"
class IEnemy;

class IEnemyState
{
public:
	IEnemyState() {};
	virtual ~IEnemyState() {};
	virtual void Enter(IEnemy* e) = 0;
	virtual IEnemyState* Update(IEnemy* e) = 0;
	virtual void Exit(IEnemy* e) = 0;

	virtual void  OnAttacked(IEnemy* e) {}

protected:

	float Approach(float goal, float current, float dt) {
		auto diff = goal - current;
		if (diff > dt) {
			return current + dt;
		}
		if (diff < -dt) {
			return current - dt;
		}
		return goal;
	}

};

