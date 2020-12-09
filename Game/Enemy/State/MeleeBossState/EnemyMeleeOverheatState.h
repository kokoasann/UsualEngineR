#pragma once
#include "../IEnemyState.h"

class EnemyMeleeOverheatState : public IEnemyState
{
public:
	EnemyMeleeOverheatState();
	virtual ~EnemyMeleeOverheatState() override;
	virtual void Enter(IEnemy* e) override;
	virtual IEnemyState* Update(IEnemy* e) override;
	virtual void Exit(IEnemy* e) override;
private:
	float m_timer = 0.f;
	float m_time = 4.5f;
};