#pragma once
#include "IEnemyState.h"

class EnemyShortRangeLongJampAttackState final : public IEnemyState
{
public:
	EnemyShortRangeLongJampAttackState();
	~EnemyShortRangeLongJampAttackState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
private:
	float m_timer = 0.f;
	float m_timeLimit = 3.f;

	float m_jampHeight = 10.f;

	float m_fowardMove = 10.f;

	Vector3 m_foward = Vector3::Zero;

	bool m_isTakOff = false;
};
