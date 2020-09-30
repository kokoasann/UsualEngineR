#pragma once
class IEnemy;

class IEnemyState
{
public:
	IEnemyState() {};
	virtual ~IEnemyState() {};
	virtual void Enter(IEnemy* e) = 0;
	virtual IEnemyState* Update(IEnemy* e) = 0;
	virtual void Exit(IEnemy* e) = 0;
};

