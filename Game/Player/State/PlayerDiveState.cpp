#include "stdafx.h"
#include "PlayerDiveState.h"
#include "../Player.h"

PlayerDiveState::PlayerDiveState()
{

}

PlayerDiveState::~PlayerDiveState()
{

}

void PlayerDiveState::Enter(Player* p) {
#ifdef _PRINT_PLAYER_STATE
	DebugPrint_WATA("Player : Enter Dive\n");
#endif

	//スタミナがなくてダイブできないにゃん！
	if (p->GetCurrentEndurance() < m_DIVE_STAMINA_COST) {
		m_isCanDive = false;
		return;
	}
	else {
		m_isCanDive = true;
	}


	auto lxf = g_pad[0]->GetLStickXF();
	auto lyf = g_pad[0]->GetLStickYF();

	if (lxf == 0.f and lyf == 0.f) {
		m_isCanDive = false;
		return;
	}

	p->UseStamina(m_DIVE_STAMINA_COST);
	m_velocity = p->GetLocalVelocity();

	m_velocity.Normalize();
	m_velocity *= 10.f;

	m_velocityGoal.x = lxf * m_VELOCITY_MAX;
	m_velocityGoal.z = lyf * m_VELOCITY_MAX;
}

IPlayerState* PlayerDiveState::Update(Player* p) {

	if (!m_isCanDive) {
		auto nextState = p->GetState(Player::EnState::enGround);
		return nextState;
	}

	auto delta = gameTime()->GetDeltaTime();

	float height = 0.f;
	float speedFraction = 1.f;
	m_velocityGoal.y = height;
	m_velocityGoal *= speedFraction;

	if (m_velocity.x == m_velocityGoal.x and m_velocity.z == m_velocityGoal.z) {
		m_velocityGoal = Vector3::Zero;
	}

	m_velocity.x = Approach(m_velocityGoal.x, m_velocity.x, delta * m_QUICKNESS);
	m_velocity.z = Approach(m_velocityGoal.z, m_velocity.z, delta * m_QUICKNESS);
	m_velocity.y = Approach(m_velocityGoal.y, m_velocity.y, delta * m_QUICKNESS);

	auto cam = g_lockCamera3D.Get();
	auto forward = cam->GetForward();
	forward.y = 0.f;
	forward.Normalize();
	Vector3 up = { 0,1,0 };
	Vector3 right = forward;
	right.Cross(up);
	right.Normalize();

	auto vel = forward * m_velocity.z + right * -m_velocity.x + up * m_velocity.y;

	if(!p->IsOnGround())
		vel.y += m_GRAVITY;

	vel *= p->GetSpeed();

	p->SetVelocity(vel);
	p->SetLocalVelocity(m_velocity);

	//Rotation
	if (vel.x != 0.f or vel.z != 0.f) {
		Quaternion rot = Quaternion::Identity;
		auto theta = atan2(vel.x, vel.z);
		theta = theta * (180.f / Math::PI);
		rot.SetRotationDegY(theta);
		p->SetRotation(rot);
	}

	//State
	if (m_velocity.x == 0 and m_velocity.z == 0) {
		auto nextState = p->GetState(Player::EnState::enGround);
		return nextState;
	}

	return this;
}

void PlayerDiveState::Exit(Player* p) {
#ifdef _PRINT_PLAYER_STATE
	DebugPrint_WATA("Player : Exit Dive\n");
#endif
}