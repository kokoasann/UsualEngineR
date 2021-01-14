#pragma once
#include "Enemy/State/IEnemyState.h"

class BossBombCoroCoroState final : public IEnemyState
{
public:
	BossBombCoroCoroState();
	~BossBombCoroCoroState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
	void  OnAttacked(IEnemy* e);
private:
};
