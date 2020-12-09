#include "stdafx.h"
#include "EnemyStunState.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/IEnemy.h"
#include "Player/Player.h"
#include "Enemy/Boss/BossA.h"
#include "GameManager.h"

EnemyStunState::EnemyStunState() {

}

EnemyStunState::~EnemyStunState() {

}

void EnemyStunState::Enter(IEnemy* e) {

	e->PlayAnimation(IEnemy::EnAnimation::enIdle);

	m_timer = 0.f;
	m_velocity = e->GetKnockBackImpulse();
	e->SetKnockBackImpulse(Vector3::Zero);
	m_velocityGoal.x = m_VELOCITY_MAX;
	m_velocityGoal.z = m_VELOCITY_MAX;

	e->SetAutoRotateFlag(false);

#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Enter melee stun\n");
#endif
}

IEnemyState* EnemyStunState::Update(IEnemy* e) {
	auto delta = gameTime()->GetDeltaTime();
	m_timer += delta;

	float height = 0.f;
	float speedFraction = 1.f;
	m_velocityGoal.y = height;

	if (m_velocity.x == m_velocityGoal.x and m_velocity.z == m_velocityGoal.z) {
		m_velocityGoal = Vector3::Zero;
	}

	m_velocity.x = Approach(m_velocityGoal.x, m_velocity.x, delta * m_QUICKNESS);
	m_velocity.z = Approach(m_velocityGoal.z, m_velocity.z, delta * m_QUICKNESS);
	m_velocity.y = Approach(m_velocityGoal.y, m_velocity.y, delta * m_QUICKNESS);

	auto vel = m_velocity;

	if (!e->IsOnGround())
		vel.y += m_GRAVITY;

	vel = m_velocity;

	e->SetVelocity(vel);

	//Rotation
	/*
	if (vel.x != 0.f or vel.z != 0.f) {
		Quaternion rot = Quaternion::Identity;
		auto theta = atan2(vel.x, vel.z);
		theta = theta * (180.f / Math::PI);
		rot.SetRotationDegY(theta);
		e->SetRotation(rot);
	}
	*/

	//State
	if (m_velocity.x == 0 and m_velocity.z == 0) {
		auto nextState = e->GetState(TO_INT(IEnemy::EnState::enBattleState));
		return nextState;
	}

	return this;
}

void EnemyStunState::Exit(IEnemy* e) {
	e->SetAutoRotateFlag(true);

#ifdef _PRINT_ENEMY_STATE
	DebugPrint_WATA("Exit melee stun\n");
#endif
}