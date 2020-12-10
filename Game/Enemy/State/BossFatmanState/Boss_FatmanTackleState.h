#pragma once
#include "Enemy/State/IEnemyState.h"

class Boss_FatmanTackleState final : public IEnemyState
{
public:
	Boss_FatmanTackleState();
	~Boss_FatmanTackleState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
private:
};
