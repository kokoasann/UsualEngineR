#include "stdafx.h"
#include "PlayerFlyingState.h"
#include "../Player.h"
#include "GameManager.h"
#include "Camera/GameCamera.h"
#include "Enemy/EnemyManager.h"
#include "HUD/KeyHelp.h"

PlayerFlyingState::PlayerFlyingState()
{

}

PlayerFlyingState::~PlayerFlyingState()
{
	if (m_keyHelp_Rise != nullptr) {
		DeleteGO(m_keyHelp_Rise);
		DeleteGO(m_keyHelp_Faster);
		DeleteGO(m_keyHelp_Switch);
	}
}

void PlayerFlyingState::Enter(Player* p) {
#ifdef _PRINT_PLAYER_STATE
	DebugPrint_WATA("Player Enter Flying\n");
#endif

	m_canRise = true;
	//ブーストの上昇初期コストがないので飛べないよ.
	if (p->GetCurrentBoost() <= m_FIRST_RISE_BOOST_COST) {
		m_canRise = false;
		return;
	}

	m_bPos = p->GetPosition();
	m_velocity = p->GetLocalVelocity();
	m_velocity.y = 0.f;

	if (p->IsOnGround()) {
		m_velocityGoal.y = m_AUTO_RISE_PARAM * m_VELOCITY_MAX;
		m_isFirstRising = true;
	}

	p->PlayAnimation(Player::EnAnimation::enHovering);
	p->FireThrusters();

	if (m_keyHelp_Rise == nullptr) {
		const float space = 50.f;
		//Rise
		m_keyHelp_Rise = NewGO<KeyHelp>(0);
		Vector3 keyHelpPos = { 200.f,-100.f,0.f };
		// TODO (FONT) : m_keyHelp_Rise->Init(keyHelpPos, L"R2:上昇");
		m_keyHelp_Rise->Init(keyHelpPos, L"R2:ジョウショウ");
		//Faster
		m_keyHelp_Faster = NewGO<KeyHelp>(0);
		Vector3 keyHelpPos2 = { 200.f,-100.f - space , 0.f };
		//m_keyHelp_Faster->Init(keyHelpPos2, L"X:加速");
		m_keyHelp_Faster->Init(keyHelpPos2, L"X:カソク");
		//SwitchToGround
		m_keyHelp_Switch = NewGO<KeyHelp>(0);
		Vector3 keyHelpPos3 = { 200.f,-100.f - space * 2,0.f };
		//m_keyHelp_Switch->Init(keyHelpPos3, L"A:落下");
		m_keyHelp_Switch->Init(keyHelpPos3, L"A:ラッカ");
	}
	else {
		m_keyHelp_Rise->SetActive(true);
		m_keyHelp_Faster->SetActive(true);
		m_keyHelp_Switch->SetActive(true);
	}
}

IPlayerState*  PlayerFlyingState::Update(Player* p) {

	//State
	auto& preset = p->GetCurrentAttackPreset();
	if (preset == Player::EnAttackPreset::enRemoteAttackPreset and g_pad[0]->IsPress(EnButton::enButtonB)) {
		m_hipShot.GenerateProjectiles(p);
	}
	else if (g_pad[0]->IsTrigger(EnButton::enButtonB) or g_pad[0]->IsTrigger(EnButton::enButtonY)) {
		auto nextState = p->GetState(Player::EnState::enAttack);
		return nextState;
	}

	if (g_pad[0]->IsTrigger(EnButton::enButtonA) or  //飛行モード解除.
		p->GetCurrentBoost() <= 0.f or //ブースト不足.
		!m_canRise //初期上昇用のブーストが不足.
		) {
		auto nextState = p->GetState(Player::EnState::enFall);
		return nextState;
	}
	
	//Move
	auto lxf = g_pad[0]->GetLStickXF();
	auto lyf = g_pad[0]->GetLStickYF();
	auto r2f = g_pad[0]->GetR2Button();

	if (p->GetPosition().y > m_bPos.y + m_rise and m_isFirstRising) {
		m_velocityGoal.y = 0.f;
		m_isFirstRising = false;
	}

	if (!m_isFirstRising) {
		m_velocityGoal.y = r2f * m_VELOCITY_MAX;
	}

	m_velocityGoal.x = lxf * m_VELOCITY_MAX;
	m_velocityGoal.z = lyf * m_VELOCITY_MAX;

	bool isMovingFaster = false;

	//BOOST
	if (g_pad[0]->IsPress(enButtonX)) {
		m_velocityGoal *= m_VELOCITY_BOOST;
		p->UseBoost(m_BOOST_RUN_EFFICIENCY *  gameTime()->GetDeltaTime());
		isMovingFaster = true;
	}
	else {
		p->UseBoost(m_BOOST_EFFICIENCY * gameTime()->GetDeltaTime());
	}

	//上昇してるなら追加でブーストを消費
	if (m_velocityGoal.y > 0.f) {
		p->UseBoost(m_RISE_BOOST_EFFICIENCY * gameTime()->GetDeltaTime());
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

	//距離チェック.
	const float Input_Ignore_Dist = 10.f;
	auto gc = GameManager::GetInstance().GetGameCamera();
	const bool isTargettingEnemy = gc->IsTargettingEnemy();
	if (isTargettingEnemy) {
		const auto target = EnemyManager::GetEnemyManager().GetTargettingEnemy();
		if (target != nullptr) {
			Vector3 nextPos = p->GetPosition() + vel;
			nextPos.y = 0.f;
			auto tar = target->GetPosition() + target->GetVelocity();
			tar.y = 0.f;

			const float nexLen = (nextPos - tar).Length();
			const float currentLen = (p->GetPosition() - tar).Length();

			if (nexLen <= Input_Ignore_Dist and nexLen < currentLen) {
				vel = Vector3::Zero;
				m_velocity = Vector3::Zero;
			}
		}
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

		if (isMovingFaster) {
			p->PlayAnimation(Player::EnAnimation::enFlyMoveFaster);
		}
		else {
			p->PlayAnimation(Player::EnAnimation::enFlyMove);
		}
	}
	else {
		p->PlayAnimation(Player::EnAnimation::enHovering);
	}

	p->ChargeEndurance(m_ENDURANCE_AUTO_CHARGE_AMOUNT * gameTime()->GetDeltaTime());

	return this;
}

void PlayerFlyingState::Exit(Player* p) {
#ifdef _PRINT_PLAYER_STATE
	DebugPrint_WATA("Player Exit Flying\n");
#endif
	m_velocity = Vector3::Zero;
	m_keyHelp_Rise->SetActive(false);
	m_keyHelp_Faster->SetActive(false);
	m_keyHelp_Switch->SetActive(false);
}