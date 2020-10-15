#include "stdafx.h"
#include "PlayerFlyingState.h"
#include "../Player.h"

PlayerFlyingState::PlayerFlyingState()
{

}

PlayerFlyingState::~PlayerFlyingState()
{

}

void PlayerFlyingState::Enter(Player* p) {
	printf("Enter Flying\n");

	m_bPos = p->GetPosition();
	m_velocity = p->GetLocalVelocity();
	m_velocityGoal.y = 100 * m_VELOCITY_MAX;

}

IPlayerState*  PlayerFlyingState::Update(Player* p) {

	//Move
	auto lxf = g_pad[0]->GetLStickXF();
	auto lyf = g_pad[0]->GetLStickYF();

	m_velocityGoal.x = lxf * m_VELOCITY_MAX;
	m_velocityGoal.z = lyf * m_VELOCITY_MAX;

	if (p->GetPosition().y > m_bPos.y + m_rise) {
		m_velocityGoal.y = 0.f;
	}

	//BOOST

	if (g_pad[0]->IsPress(enButtonX)) {
		m_velocityGoal *= m_VELOCITY_BOOST;
		p->UseBoost(m_BOOST_EFFICIENCY * m_ACCELERATE_PARAM * gameTime()->GetDeltaTime());
	}
	else {
		p->UseBoost(m_BOOST_EFFICIENCY * gameTime()->GetDeltaTime());
	}

	auto delta = gameTime()->GetDeltaTime();

	auto cam = g_lockCamera3D.Get();
	auto forward = cam->GetForward();
	forward.y = 0.f;
	forward.Normalize();
	Vector3 up = { 0,1,0 };
	Vector3 right = forward;
	right.Cross(up);
	right.Normalize();

	m_velocity.x = Approach(m_velocityGoal.x, m_velocity.x, delta * m_QUICKNESS);
	m_velocity.z = Approach(m_velocityGoal.z, m_velocity.z, delta * m_QUICKNESS);
	m_velocity.y = Approach(m_velocityGoal.y, m_velocity.y, delta * m_QUICKNESS);

	auto vel = forward * m_velocity.z + right * -m_velocity.x + up * m_velocity.y;

	vel *= p->GetSpeed() * gameTime()->GetDeltaTime();
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
	if (g_pad[0]->IsTrigger(EnButton::enButtonA)) {
		auto nextState = p->GetState(Player::EnState::enGround);
		return nextState;
	}
	return this;
}

void PlayerFlyingState::Exit(Player* p) {
	m_velocity = Vector3::Zero;
	printf("Exit Flying\n");
}