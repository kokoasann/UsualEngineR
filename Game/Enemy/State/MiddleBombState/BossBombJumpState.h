#pragma once
#include "Enemy/State/IEnemyState.h"

class BossBombJumpState final : public IEnemyState
{
public:
	BossBombJumpState();
	~BossBombJumpState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
	void  OnAttacked(IEnemy* e);
private:
};
