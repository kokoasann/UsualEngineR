#pragma once
#include "Enemy/State/IEnemyState.h"

class BossBombFlyingBattleState final : public IEnemyState
{
public:
	BossBombFlyingBattleState();
	~BossBombFlyingBattleState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
	void  OnAttacked(IEnemy* e);
private:
};
