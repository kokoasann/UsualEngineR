#pragma once
#include "Enemy/State/IEnemyState.h"

class Boss_FatmanChargeBeamAndShootingState final : public IEnemyState
{
public:
	Boss_FatmanChargeBeamAndShootingState();
	~Boss_FatmanChargeBeamAndShootingState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
	void  OnAttacked(IEnemy* e);
private:
};
