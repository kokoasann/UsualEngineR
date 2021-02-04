#pragma once
#include "Enemy/State/IEnemyState.h"

class BossBombIdleState final : public IEnemyState
{
public:
	BossBombIdleState();
	~BossBombIdleState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
	void  OnAttacked(IEnemy* e);
private:
	float m_timer = 0.f;
	float m_timeRotate = 0.5;

	Quaternion m_firstRot = Quaternion::Identity;

	bool m_isAttacked = false;

	const float m_ikMoveSpeed = 20.f;
	Vector3 m_oldIKPos = Vector3::Zero;
};
