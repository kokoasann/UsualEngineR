#pragma once
#include "IEnemyState.h"

class ExplosionEffect;
class EnemyLongDistanceBlownState final : public IEnemyState
{
public:
	EnemyLongDistanceBlownState();
	~EnemyLongDistanceBlownState() override;
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
private:
	IK* m_ik[6] = { nullptr };
	ExplosionEffect* m_effect = nullptr;
	bool m_isPlayEffect = false;
	Vector3 m_velocityXZ = Vector3::Zero;
	float m_velocityY = 0.f;
	bool m_isNoBlown = false;

	float m_grav = 150.f;

	float m_timer = 0.;
	float m_timeLimit = 2.f;

	bool m_isTakeOff = false;
	bool m_isTakeOn = false;

	RigidBody* m_body = nullptr;
};
