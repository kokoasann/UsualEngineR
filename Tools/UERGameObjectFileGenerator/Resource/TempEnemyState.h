#pragma once
#include "IEnemyState.h"

class XXX final : public IEnemyState
{
public:
	XXX();
	~XXX();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
:block OnAttacked
	void  OnAttacked(IEnemy* e);
:end
private:
};