#pragma once
#include "IEnemyState.h"

class EnemyShortRangeStunState final : public IEnemyState
{
public:
	EnemyShortRangeStunState();
	~EnemyShortRangeStunState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
private:

	float m_timer = 0.f;
	float m_t = 0.f;
	float m_timeLimit = 0.f;
	Vector3 m_velocity = Vector3::Zero;
	Vector3 m_velocityEnd = Vector3::Zero;

	Vector3 m_knockBack = Vector3::Zero;
	float m_up = 0.f;

	float m_grav = 100.f;

	IK* m_ik = nullptr;
};
