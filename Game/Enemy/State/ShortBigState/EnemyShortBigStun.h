#pragma once
#include "Enemy/State/IEnemyState.h"

class EnemyShortBigStun final : public IEnemyState
{
public:
	EnemyShortBigStun();
	~EnemyShortBigStun();
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
