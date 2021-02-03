#include "stdafx.h"
#include "PlayerStunState.h"
#include "../Player.h"
#include "../../GameManager.h"

PlayerStunState::PlayerStunState()
{

}

PlayerStunState::~PlayerStunState()
{

}

void PlayerStunState::Enter(Player* p) {

	m_timer = 0.f;

	m_velocity = p->m_knockBackImpulse;

	m_velFriction = m_velocity;
	m_velFriction.y = 0;
	m_velFriction.Normalize();
	m_velFriction *= -m_friction;

	//m_velocity.Normalize();
	//m_velocity *= m_FIRST_VEL_PARAM;

	p->PlayAnimation(Player::EnAnimation::enKnockBack);

	//m_velocityGoal.x =  m_VELOCITY_MAX;
	//m_velocityGoal.z =  m_VELOCITY_MAX;


#ifdef _PRINT_PLAYER_STATE
	DebugPrint_WATA("Player : Enter Stun\n");
#endif
}

IPlayerState* PlayerStunState::Update(Player* p) {

	//auto delta = gameTime()->GetDeltaTime();
	//m_timer += delta;

	//float height = 0.f;
	//float speedFraction = 1.f;
	//m_velocityGoal.y = height;

	//if (m_velocity.x == m_velocityGoal.x and m_velocity.z == m_velocityGoal.z) {
	//	m_velocityGoal = Vector3::Zero;
	//}

	//m_velocity.x = Approach(m_velocityGoal.x, m_velocity.x, delta * m_QUICKNESS);
	//m_velocity.z = Approach(m_velocityGoal.z, m_velocity.z, delta * m_QUICKNESS);
	//m_velocity.y = Approach(m_velocityGoal.y, m_velocity.y, delta * m_QUICKNESS);

	//auto vel = m_velocity;

	//if (!p->IsOnGround())
	//	vel.y += m_GRAVITY;

	//vel = m_velocity;

	//p->SetVelocity(vel);

	//////Rotation
	////if (vel.x != 0.f or vel.z != 0.f) {
	////	Quaternion rot = Quaternion::Identity;
	////	auto theta = atan2(vel.x, vel.z);
	////	theta = theta * (180.f / Math::PI);
	////	rot.SetRotationDegY(theta);
	////	p->SetRotation(rot);
	////}

	////State
	//if (m_velocity.x == 0 and m_velocity.z == 0) {
	//	auto nextState = p->GetState(Player::EnState::enGround);
	//	return nextState;
	//}

	//return this;

	float dtime = gameTime()->GetDeltaTime();

	m_velocity.y -= m_GRAVITY * dtime;
	if (p->IsOnGround())
	{
		float velXZLen = sqrtf(powf(m_velocity.x, 2) + powf(m_velocity.z, 2));

		if (velXZLen > m_friction)
		{
			m_velocity += m_velFriction * dtime;
		}
		else
		{
			p->SetExternalVelocity(Vector3::Zero);
			return p->GetState(Player::EnState::enGround);
		}
	}
	p->SetExternalVelocity(m_velocity);
	return this;
}

void PlayerStunState::Exit(Player* p) {
#ifdef _PRINT_PLAYER_STATE
	DebugPrint_WATA("Player : Exit Stun\n");
#endif
}