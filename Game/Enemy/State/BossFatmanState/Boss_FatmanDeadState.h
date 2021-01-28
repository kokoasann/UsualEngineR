#pragma once
#include "Enemy/State/IEnemyState.h"

class Boss_FatmanDeadState final : public IEnemyState
{
public:
	Boss_FatmanDeadState();
	~Boss_FatmanDeadState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
	void  OnAttacked(IEnemy* e);
private:
};
