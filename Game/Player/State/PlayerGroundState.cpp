#include "stdafx.h"
#include "PlayerGroundState.h"
#include "../../Camera/GameCamera.h"
#include <cmath>

PlayerGroundState::PlayerGroundState()
{
}

PlayerGroundState::~PlayerGroundState()
{
}

void PlayerGroundState::Enter(Player* p){
	m_velocity = p->GetLocalVelocity();
	p->PlayAnimation(Player::EnAnimation::enRun);
	printf("Enter Ground\n");
}

IPlayerState* PlayerGroundState::Update(Player* p) {

	//Move
	if(p->GetGameCamera().IsTargettingEnemy()){
		TargettingEnemyMove(p);
	}
	else {
		CameraWorldMove(p);
	}
	
	//State
	if (g_pad[0]->IsTrigger(EnButton::enButtonB)) {
		auto nextState = p->GetState(Player::EnState::enAttack);
		return nextState;
	}

	if (g_pad[0]->IsTrigger(EnButton::enButtonA)) {
		 auto nextState =  p->GetState(Player::EnState::enFlying);
		return nextState;
	}

	if (g_pad[0]->IsTrigger(EnButton::enButtonRB1)){
		auto nextState = p->GetState(Player::EnState::enDiving);
		return nextState;
	}

	//boost recharge
	p->ChargeBoost(m_BOOST_AUTO_CHARGE_AMOUNT * gameTime()->GetDeltaTime());
	//endurance recharge
	p->ChargeEndurance(m_ENDURANCE_AUTO_CHARGE_AMOUNT * gameTime()->GetDeltaTime());

	return this;
}

void PlayerGroundState::Exit(Player* p) {
	printf("Exit Ground\n");
}


void PlayerGroundState::TargettingEnemyMove(Player* p) {
	auto lxf = g_pad[0]->GetLStickXF();
	auto lyf = g_pad[0]->GetLStickYF();

	bool isRunning = false;

	//m_vecVelocityGoal.x = lxf * m_VELOCITY_MAX;
	//m_vecVelocityGoal.z = lyf * m_VELOCITY_MAX;
	m_vecVelocityGoal.x = lxf;
	m_vecVelocityGoal.z = lyf;

	if (g_pad[0]->IsPress(enButtonX)) {
		//TODO : スタミナがないと走れないようにする
		m_vecVelocityGoal *= m_RUN_SPEED_PARAM * m_VELOCITY_MAX;
		isRunning = true;
		p->UseStamina(m_RUN_COST * gameTime()->GetDeltaTime());
	}
	else {
		m_vecVelocityGoal *= m_SIDE_MOVE_VELOCITY_MAX;
	}

	auto delta = gameTime()->GetDeltaTime();
	m_velocity.x = Approach(m_vecVelocityGoal.x, m_velocity.x, delta * m_QUICKNESS);
	m_velocity.z = Approach(m_vecVelocityGoal.z, m_velocity.z, delta * m_QUICKNESS);
	auto cam = g_lockCamera3D.Get();

	auto forward = cam->GetForward();

	forward.y = 0.f;
	forward.Normalize();
	Vector3 up = { 0,1,0 };
	Vector3 right = forward;
	right.Cross(up);
	right.Normalize();

	auto vel = forward * m_velocity.z + right * -m_velocity.x;

	if (!p->IsOnGround()) {
		vel.y += m_GRAVITY;
	}

	vel *= p->GetSpeed();// *gameTime()->GetDeltaTime();
	p->SetVelocity(vel);
	p->SetLocalVelocity(m_velocity);

	//Rotation
	if (isRunning and (vel.x != 0.f or vel.z != 0.f)) {
		Quaternion rot = Quaternion::Identity;
		auto theta = atan2(vel.x, vel.z);
		theta = theta * (180.f / Math::PI);
		rot.SetRotationDegY(theta);
		p->SetRotation(rot);

		p->PlayAnimation(Player::EnAnimation::enRun);

	}
	else {
		auto cf = g_camera3D->GetForward();
		Quaternion rot = Quaternion::Identity;
		auto theta = atan2(cf.x, cf.z);
		theta = theta * (180.f / Math::PI);
		rot.SetRotationDegY(theta);
		p->SetRotation(rot);
		p->PlayAnimation(Player::EnAnimation::enIdle);
	}

}

void PlayerGroundState::CameraWorldMove(Player* p) {
	auto lxf = g_pad[0]->GetLStickXF();
	auto lyf = g_pad[0]->GetLStickYF();

	m_vecVelocityGoal.x = lxf * m_VELOCITY_MAX;
	m_vecVelocityGoal.z = lyf * m_VELOCITY_MAX;

	if (g_pad[0]->IsPress(enButtonX)) {
		m_vecVelocityGoal *= m_RUN_SPEED_PARAM;
		p->UseStamina(m_RUN_COST * gameTime()->GetDeltaTime());
	}

	auto delta = gameTime()->GetDeltaTime();
	m_velocity.x = Approach(m_vecVelocityGoal.x, m_velocity.x, delta * m_QUICKNESS);
	m_velocity.z = Approach(m_vecVelocityGoal.z, m_velocity.z, delta * m_QUICKNESS);

	auto cam = g_lockCamera3D.Get();
	auto forward = cam->GetForward();

	forward.y = 0.f;
	forward.Normalize();
	Vector3 up = { 0,1,0 };
	Vector3 right = forward;
	right.Cross(up);
	right.Normalize();

	auto vel = forward * m_velocity.z + right * -m_velocity.x;

	if (!p->IsOnGround()) {
		vel.y += m_GRAVITY;
	}

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
}