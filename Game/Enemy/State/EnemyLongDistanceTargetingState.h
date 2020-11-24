#pragma once
#include "IEnemyState.h"

class EnemyLongDistanceTargetingState final : public IEnemyState
{
public:
	EnemyLongDistanceTargetingState();
	~EnemyLongDistanceTargetingState();

	void Init(IK* ik, Quaternion& rot);

	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
private:
	IK* m_ik = nullptr;
	float m_timer = 0.f;
	const float m_timeLimit = 2.f;

	Quaternion* m_rot = nullptr;
};
