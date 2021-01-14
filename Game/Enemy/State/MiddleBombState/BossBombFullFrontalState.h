#pragma once
#include "Enemy/State/IEnemyState.h"

class BossBombFullFrontalState final : public IEnemyState
{
public:
	BossBombFullFrontalState();
	~BossBombFullFrontalState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
	void  OnAttacked(IEnemy* e);
private:
};
