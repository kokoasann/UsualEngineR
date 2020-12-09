#pragma once
#include "Enemy/State/IEnemyState.h"

class EnemyStunState : public IEnemyState
{
public:
	EnemyStunState();
	virtual ~EnemyStunState() override;
	virtual void Enter(IEnemy* e) override;
	virtual IEnemyState* Update(IEnemy* e) override;
	virtual void Exit(IEnemy* e) override;
private:
	float m_timer = 0.f;
	const float m_stunTime = 1.f;

	const float m_VELOCITY_MAX = 10.f;
	const float m_GRAVITY = -9.81f;
	const float m_QUICKNESS = 300.f;
	const float m_FIRST_VEL_PARAM = 15.f;

	Vector3 m_velocity = Vector3::Zero;
	Vector3 m_velocityGoal = Vector3::Zero;
};