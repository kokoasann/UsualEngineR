#pragma once
#include "../IEnemyState.h"

class EnemyMeleeFlyState : public IEnemyState
{
public:
	EnemyMeleeFlyState();
	virtual ~EnemyMeleeFlyState() override;
	virtual void Enter(IEnemy* e) override;
	virtual IEnemyState* Update(IEnemy* e) override;
	virtual void Exit(IEnemy* e) override;
private:
	float m_flyTimer = 0.f;
	const float m_flyLimitTime = 5.f;
};