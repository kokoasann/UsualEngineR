#pragma once
#include "Enemy/State/IEnemyState.h"
#include "EnemyLongBigAttackState.h"

class EnemyLongBigTargetingState final : public IEnemyState
{
public:
	EnemyLongBigTargetingState();
	~EnemyLongBigTargetingState();
	void Init(IK* ik, Quaternion& rot, float bulletSpeed);

	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
private:
	EnemyLongBigAttackState m_lba;

	IK* m_ik = nullptr;
	float m_timer = 0.f;
	const float m_timeLimit = 2.f;
	Quaternion* m_rot = nullptr;
	float m_speed = 0.f;

	float m_overLookRange = 300.f;
};
