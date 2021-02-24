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
	p->SetExternalVelocity(Vector3::Zero);
}