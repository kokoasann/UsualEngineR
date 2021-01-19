#pragma once
#include "Enemy/State/IEnemyState.h"

class Boss_FatmanStunState final : public IEnemyState
{
public:
	Boss_FatmanStunState();
	~Boss_FatmanStunState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
	void  OnAttacked(IEnemy* e);
private:
};
