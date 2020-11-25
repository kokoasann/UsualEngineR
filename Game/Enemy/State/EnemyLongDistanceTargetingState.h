#pragma once
#include "IEnemyState.h"
#include "EnemyLongDistanceAttackState.h"

class EnemyLongDistanceTargetingState final : public IEnemyState
{
public:
	EnemyLongDistanceTargetingState();
	~EnemyLongDistanceTargetingState();

	void Init(IK* ik, Quaternion& rot, float bulletSpeed);

	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
private:
	EnemyLongDistanceAttackState m_lda;

	IK* m_ik = nullptr;
	float m_timer = 0.f;
	const float m_timeLimit = 2.f;
	Quaternion* m_rot = nullptr;
	float m_speed = 0.f;

	float m_overLookRange = 300.f;
};
