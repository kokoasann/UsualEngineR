#pragma once
#include "Enemy/State/IEnemyState.h"

class BossBombBashState final : public IEnemyState
{
public:
	BossBombBashState();
	~BossBombBashState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
	void  OnAttacked(IEnemy* e);
	void Init(IEnemy* e);
private:
	float m_timer = 0.;
	float m_timeLimit = 2.;
	SphereCollider m_sphere;

	Vector3 m_dir;
	float m_speed = 100.f;
	bool m_isBashHit = false;
};
