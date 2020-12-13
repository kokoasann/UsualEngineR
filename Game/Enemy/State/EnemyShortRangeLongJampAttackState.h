#pragma once
#include "IEnemyState.h"
#include "Player/Player.h"

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
	float m_timeLimit = 1.f;

	float m_jampHeight = 80.f;

	float m_fowardMove = 100.f;

	Vector3 m_foward = Vector3::Zero;

	float m_grav = 400.f;
	float m_gravPow = 0.f;
	bool m_isTakOff = false;

	float m_damage = Player::GetMaxHP() / 30.0f;
	bool m_isApplyDamage = false;
};
