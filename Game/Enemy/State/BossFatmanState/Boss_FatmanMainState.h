#pragma once
#include "Enemy/State/IEnemyState.h"

class Boss_FatmanMainState final : public IEnemyState
{
public:
	Boss_FatmanMainState();
	~Boss_FatmanMainState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
private:
};
