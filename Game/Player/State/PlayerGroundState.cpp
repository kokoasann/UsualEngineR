#include "stdafx.h"
#include "PlayerGroundState.h"
#include "../../Camera/GameCamera.h"
#include "../../GameManager.h"
#include <cmath>

PlayerGroundState::PlayerGroundState()
{
}

PlayerGroundState::~PlayerGroundState()
{
}

void PlayerGroundState::Enter(Player* p){

	p->StopThrusters();

	m_velocity = p->GetLocalVelocity();
	p->PlayAnimation(Player::EnAnimation::enWalk, m_AnimInterpolate);
#ifdef _PRINT_PLAYER_STATE
	DebugPrint_WATA("Enter Ground\n");
#endif
}

IPlayerState* PlayerGroundState::Update(Player* p) {

	//Move
	const bool isEnemyCamera = GameManager::GetInstance().m_camera->IsTargettingEnemy();
	if(isEnemyCamera){
		TargettingEnemyMove(p);
	}
	else {
		CameraWorldMove(p);
	}
	
	//State
	if (g_pad[0]->IsTrigger(EnButton::enButtonB) or g_pad[0]->IsTrigger(EnButton::enButtonY)) {
		auto nextState = p->GetState(Player::EnState::enAttack);
		return nextState;
	}

	if (g_pad[0]->IsTrigger(EnButton::enButtonA)) {
		 auto nextState =  p->GetState(Player::EnState::enFlying);
		return nextState;
	}

	if (g_pad[0]->IsTrigger(EnButton::enButtonLB1)){
		auto nextState = p->GetState(Player::EnState::enDiving);
		return nextState;
	}

	if (g_pad[0]->IsTrigger(EnButton::enButtonLB2)) {
		auto nextState = p->GetState(Player::EnState::enGuard);
		return nextState;
	}

	//boost recharge
	p->ChargeBoost(m_BOOST_AUTO_CHARGE_AMOUNT * gameTime()->GetDeltaTime());

	return this;
}

void PlayerGroundState::Exit(Player* p) {
	p->GetModelRender().SetAnimPlaySpeed(m_DefaultAnimSpeed);
#ifdef _PRINT_PLAYER_STATE
	DebugPrint_WATA("Exit Ground\n");
#endif
}


void PlayerGroundState::TargettingEnemyMove(Player* p) {
	auto lxf = g_pad[0]->GetLStickXF();
	auto lyf = g_pad[0]->GetLStickYF();

	bool isRunning = false;

	//m_vecVelocityGoal.x = lxf * m_VELOCITY_MAX;
	//m_vecVelocityGoal.z = lyf * m_VELOCITY_MAX;
	m_vecVelocityGoal.x = lxf;
	m_vecVelocityGoal.z = lyf;



	if (g_pad[0]->IsTrigger(enButtonX)) {
		if (p->GetCurrentEndurance() > m_RUN_BEGIN_COST) {
			m_hasEnoughStaminaToRun = true;
		}
	}

	//押しっぱなしでスタミナが切れた場合は再度ボタンを押しなおすまで走らない.
	if (g_pad[0]->IsPress(enButtonX) and m_hasEnoughStaminaToRun) {
		if (p->GetCurrentEndurance() > m_RUN_BEGIN_COST) {
			m_vecVelocityGoal *= m_RUN_SPEED_PARAM * m_VELOCITY_MAX;
			isRunning = true;
			p->PlayAnimation(Player::EnAnimation::enRun, m_AnimInterpolate);
			p->GetModelRender().SetAnimPlaySpeed(m_DefaultAnimSpeed);
		}
		else {
			m_hasEnoughStaminaToRun = false;
		}
	}
	else {
		m_vecVelocityGoal *= m_SIDE_MOVE_VELOCITY_MAX;
		p->PlayAnimation(Player::EnAnimation::enIdle, m_AnimInterpolate);
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
		p->UseStamina(m_RUN_COST * gameTime()->GetDeltaTime());

		Quaternion rot = Quaternion::Identity;
		auto theta = atan2(vel.x, vel.z);
		theta = theta * (180.f / Math::PI);
		rot.SetRotationDegY(theta);
		p->SetRotation(rot);

	}
	else {
		auto cf = g_camera3D->GetForward();
		Quaternion rot = Quaternion::Identity;
		auto theta = atan2(cf.x, cf.z);
		theta = theta * (180.f / Math::PI);
		rot.SetRotationDegY(theta);
		p->SetRotation(rot);
	}

	//Stamina
	if (!isRunning) {
		//endurance recharge
		p->ChargeEndurance(m_ENDURANCE_AUTO_CHARGE_AMOUNT * gameTime()->GetDeltaTime());
	}

}

void PlayerGroundState::CameraWorldMove(Player* p) {

	auto lxf = g_pad[0]->GetLStickXF();
	auto lyf = g_pad[0]->GetLStickYF();

	m_vecVelocityGoal.x = lxf * m_VELOCITY_MAX;
	m_vecVelocityGoal.z = lyf * m_VELOCITY_MAX;

	Vector3 stick(lxf, lyf, 0.f);
	float animSpeed = m_DefaultAnimSpeed + stick.Length();

	bool isRunning = false;

	if (g_pad[0]->IsTrigger(enButtonX)) {
		if (p->GetCurrentEndurance() > m_RUN_BEGIN_COST) {
			m_hasEnoughStaminaToRun = true;
		}
	}
	//押しっぱなしでスタミナが切れた場合は再度ボタンを押しなおすまで走らない.
	if (g_pad[0]->IsPress(enButtonX) and m_hasEnoughStaminaToRun) {
		if (p->GetCurrentEndurance() > m_RUN_BEGIN_COST) {
			isRunning = true;
			m_vecVelocityGoal *= m_RUN_SPEED_PARAM;
			p->PlayAnimation(Player::EnAnimation::enRun, m_AnimInterpolate);
			p->GetModelRender().SetAnimPlaySpeed(m_DefaultAnimSpeed);
		}
		else {
			m_hasEnoughStaminaToRun = false;
		}
	}
	else {
		p->PlayAnimation(Player::EnAnimation::enWalk, m_AnimInterpolate);
		p->GetModelRender().SetAnimPlaySpeed(animSpeed);
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

	if (vel.x == 0.f and vel.z == 0.f) {
		p->PlayAnimation(Player::EnAnimation::enIdle, m_AnimInterpolate);
	}else if(isRunning){
		p->UseStamina(m_RUN_COST * gameTime()->GetDeltaTime());
	}

	//Rotation
	if (vel.x != 0.f or vel.z != 0.f) {
		Quaternion rot = Quaternion::Identity;
		auto theta = atan2(vel.x, vel.z);
		theta = theta * (180.f / Math::PI);
		rot.SetRotationDegY(theta);
		p->SetRotation(rot);
	}

	//Stamina
	if (!isRunning) {
		//endurance recharge
		p->ChargeEndurance(m_ENDURANCE_AUTO_CHARGE_AMOUNT * gameTime()->GetDeltaTime());
	}
}