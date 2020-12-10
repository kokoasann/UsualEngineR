#pragma once
#include "Enemy/State/IEnemyState.h"

class Boss_FatmanShootingState final : public IEnemyState
{
public:
	Boss_FatmanShootingState();
	~Boss_FatmanShootingState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
private:
};
