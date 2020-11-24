#pragma once
#include "Enemy/State/IEnemyState.h"

class EnemyLongDistanceAttackState final :public IEnemyState
{
public:
	

	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;

	void SetVelocity(Vector3& v)
	{
		m_velocity = std::move(v);
	}
	void SetTarget(Vector3&& v)
	{
		m_target = v;
	}
	void SetIK(IK* ik)
	{
		m_ik = ik;
	}
private:
	IK* m_ik = nullptr;
	float m_timer = 0.f;
	const float m_timeSpan = 0.1f;
	int m_shotCount = 0;
	const int m_maxShot = 3;

	Vector3 m_target = Vector3::Zero;

	Vector3 m_velocity = Vector3::Zero;
	float m_speed = 500.f;
};