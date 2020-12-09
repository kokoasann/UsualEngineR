#pragma once
#include "../IEnemyState.h"

class EnemyMeleeDeadState : public IEnemyState
{
public:
	EnemyMeleeDeadState();
	virtual ~EnemyMeleeDeadState() override;
	virtual void Enter(IEnemy* e) override;
	virtual IEnemyState* Update(IEnemy* e) override;
	virtual void Exit(IEnemy* e) override;
private:
	bool m_isPerformed = false;
	Vector3 m_pos = Vector3::Zero;
};