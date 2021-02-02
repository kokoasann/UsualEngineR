#include "stdafx.h"
#include "PlayerFallState.h"
#include "../../Camera/GameCamera.h"
#include "HUD/KeyHelp.h"
#include <cmath>

PlayerFallState::PlayerFallState()
{
}

PlayerFallState::~PlayerFallState()
{
	if (m_keyHelp_brake != nullptr) {
		DeleteGO(m_keyHelp_brake);
	}
}

void PlayerFallState::Enter(Player* p){

	//p->StopThrusters();

	m_velocity = p->GetLocalVelocity();
	p->PlayAnimation(Player::EnAnimation::enIdle, m_AnimInterpolate);
#ifdef _PRINT_PLAYER_STATE
	DebugPrint_WATA("Player Enter Fall State\n");
#endif
	if (m_keyHelp_brake == nullptr) {
		m_keyHelp_brake = NewGO<KeyHelp>(0);
		Vector3 keyHelpPos = { 200.f,-100.f,0.f };
		m_keyHelp_brake->Init(keyHelpPos, L"R2:Œ¸‘¬");
	}
	else {
		m_keyHelp_brake->SetActive(true);
	}

}

IPlayerState* PlayerFallState::Update(Player* p) {

	if (p->IsOnGround()) {
		auto nextState = p->GetState(Player::EnState::enGround);
		return nextState;
	}

	if (g_pad[0]->IsTrigger(EnButton::enButtonA)) {
		auto nextState = p->GetState(Player::EnState::enFlying);
		return nextState;
	}

	//Move
	auto lxf = g_pad[0]->GetLStickXF();
	auto lyf = g_pad[0]->GetLStickYF();
	auto r2f = g_pad[0]->GetR2Button();

	m_velocityGoal.x = lxf * m_VELOCITY_MAX;
	m_velocityGoal.z = lyf * m_VELOCITY_MAX;
	if (g_pad[0]->IsPress(enButtonRB2)) {
		m_velocityGoal.y = m_GRAVITY * 0.25f;
	}
	else {
		m_velocityGoal.y = m_GRAVITY;
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

	p->ChargeEndurance(m_ENDURANCE_AUTO_CHARGE_AMOUNT * gameTime()->GetDeltaTime());

	return this;
}

void PlayerFallState::Exit(Player* p) {
#ifdef _PRINT_PLAYER_STATE
	DebugPrint_WATA("Exit FallState\n");
#endif
	m_keyHelp_brake->SetActive(false);
}
