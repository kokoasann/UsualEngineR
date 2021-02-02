#include "stdafx.h"
#include "PlayerChurchState.h"
#include "../Player.h"
#include "../../GameManager.h"
#include "Church.h"
#include "GameSceneMenu.h";
#include "HUD/KeyHelp.h"

PlayerChurchState::PlayerChurchState()
{

}

PlayerChurchState::~PlayerChurchState()
{
	if (m_keyHelp_Heal != nullptr) {
		DeleteGO(m_keyHelp_Heal);
	}
}

void PlayerChurchState::Enter(Player* p) {
#ifdef _PRINT_PLAYER_STATE
	DebugPrint_WATA("Player : Enter Church State\n");
#endif

	p->StopThrusters();

	m_velocity = p->GetLocalVelocity();
	p->PlayAnimation(Player::EnAnimation::enWalk, m_AnimInterpolate);

	if (m_keyHelp_Heal == nullptr) {
		m_keyHelp_Heal = NewGO<KeyHelp>(0);
		Vector3 keyHelpPos = { 200.f,-100.f,0.f };
		m_keyHelp_Heal->Init(keyHelpPos, L"B:回復");
	}
	else {
		m_keyHelp_Heal->SetActive(true);
	}


}

IPlayerState* PlayerChurchState::Update(Player* p) {

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
}
	else if (isRunning) {
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

	p->ChargeBoost(m_BOOST_AUTO_CHARGE_AMOUNT* gameTime()->GetDeltaTime());

	if (g_pad[0]->IsTrigger(EnButton::enButtonB)) {
		auto church = GameManager::GetInstance().GetChurch();
		if(church->IsPossibleToHeal()){
			church->DoHeal();
			p->PlayAnimation(Player::EnAnimation::enIdle, m_AnimInterpolate);
		}
	}
	return this;
}

void PlayerChurchState::Exit(Player* p) {
#ifdef _PRINT_PLAYER_STATE
	DebugPrint_WATA("Player : Exit Church State\n");
#endif
	p->GetModelRender().SetAnimPlaySpeed(m_DefaultAnimSpeed);
	m_keyHelp_Heal->SetActive(false);
}