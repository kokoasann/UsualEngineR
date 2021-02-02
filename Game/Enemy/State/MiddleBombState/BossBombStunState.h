#pragma once
#include "Enemy/State/IEnemyState.h"

class BossBombStunState final : public IEnemyState
{
public:
	BossBombStunState();
	~BossBombStunState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
	void  OnAttacked(IEnemy* e);
private:
	Vector3 m_velocity = Vector3::Zero;
	Vector3 m_velDirXZ = Vector3::Zero;
	float m_friction = 100.f;
};
