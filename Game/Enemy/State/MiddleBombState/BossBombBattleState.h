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
	float m_timeRotate = 0.5f;

	bool m_isAngry = false;

	float m_distance = 50.f;
	float m_distanceBash = 30.f;
	float m_speed = 500.f;

	Quaternion m_firstRot;

	bool m_isPerformed = false;

};
