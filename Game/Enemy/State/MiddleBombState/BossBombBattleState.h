#pragma once
#include "Enemy/State/IEnemyState.h"

class BossBombBattleState final : public IEnemyState
{
public:
	BossBombBattleState();
	~BossBombBattleState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
	void  OnAttacked(IEnemy* e);
private:
	float m_timer = 0.f;
	float m_timeLimit = 2.f;
	bool m_isAngry = false;
};
