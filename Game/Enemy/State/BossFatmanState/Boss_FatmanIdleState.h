#pragma once
#include "Enemy/State/IEnemyState.h"

class Boss_FatmanIdleState final : public IEnemyState
{
public:
	Boss_FatmanIdleState();
	~Boss_FatmanIdleState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
	void  OnAttacked(IEnemy* e);
private:
};
