#pragma once
#include "../IEnemyState.h"

class EnemyMeleeIdleState : public IEnemyState
{
public:
	EnemyMeleeIdleState();
	virtual ~EnemyMeleeIdleState() override;
	virtual void Enter(IEnemy* e) override;
	virtual IEnemyState* Update(IEnemy* e) override;
	virtual void Exit(IEnemy* e) override;
	virtual void OnAttacked(IEnemy* e) override;
private:
	const float m_Stamina_recovery_param = 1.f;
	bool m_isAttacked = false;
};