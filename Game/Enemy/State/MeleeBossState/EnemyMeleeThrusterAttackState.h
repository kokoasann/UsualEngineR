#pragma once
#include "../IEnemyState.h"

class EnemyMeleeThrusterAttackState : public IEnemyState
{
public:
	EnemyMeleeThrusterAttackState();
	virtual ~EnemyMeleeThrusterAttackState() override;
	virtual void Enter(IEnemy* e) override;
	virtual IEnemyState* Update(IEnemy* e) override;
	virtual void Exit(IEnemy* e) override;
private:
	bool m_canExecute = true;
	//stamina
	const float m_cost = 30.f;
	const float m_range = 50.f;
	const float m_damage = 10.f;

	const float m_fireTime = 1.f;
	float m_fireTimer = 0.f;
};