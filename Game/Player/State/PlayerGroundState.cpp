#include "stdafx.h"
#include "PlayerGroundState.h"
#include "../../Camera/GameCamera.h"
#include "../../GameManager.h"
#include "HUD/KeyHelp.h"
#include <cmath>

#include "Enemy/EnemyManager.h"

PlayerGroundState::PlayerGroundState()
{
}

PlayerGroundState::~PlayerGroundState()
{
	if (m_keyHelp_Run != nullptr) {
		DeleteGO(m_keyHelp_Run);
		DeleteGO(m_keyHelp_Guard);
		DeleteGO(m_keyHelp_Dive);
	}
}

void PlayerGroundState::Enter(Player* p){
	const float space = 25.f;
	if (m_keyHelp_Run == nullptr) {
		
		//
		m_keyHelp_Run = NewGO<KeyHelp>(0);
		Vector3 keyHelpPos = { 200.f,-100.f,0.f };
		m_keyHelp_Run->Init(keyHelpPos, L"X:ダッシュ");
		//guard
		m_keyHelp_Guard = NewGO<KeyHelp>(0);
		//Vector3 keyHelpPos2 = { 200.f,-100.f - space,0.f };
		Vector3 keyHelpPos2 = { 200.f,-100.f,0.f };
		m_keyHelp_Guard->Init(keyHelpPos2, L"L2:防御");

		//dive
		m_keyHelp_Dive = NewGO<KeyHelp>(0);
		//Vector3 keyHelpPos3 = { 200.f,-100.f - space * 2,0.f };
		Vector3 keyHelpPos3 = { 200.f,-100.f - space,0.f };
		m_keyHelp_Dive->Init(keyHelpPos3, L"L1:回避");

		// target
		m_keyHelp_Target = NewGO<KeyHelp>(0);
		Vector3 keyHelpPos4 = { 200.f,-100.f,0.f };
		m_keyHelp_Target->Init(keyHelpPos4, L"R3:照準");

		m_keyHelp_Guard->SetActive(false);
		m_keyHelp_Dive->SetActive(false);

		m_keyHelp_Target->SetActive(false);

		m_keyHelp_Run->Undisplay();
		m_keyHelp_Guard->Undisplay();
		m_keyHelp_Dive->Undisplay();
		m_keyHelp_Target->Undisplay();
	}
	else {
		m_keyHelp_Run->SetActive(false);
		
		m_keyHelp_Guard->SetActive(false);
		m_keyHelp_Dive->SetActive(false);

		m_keyHelp_Target->SetActive(false);

		m_keyHelp_Run->Undisplay();
		m_keyHelp_Guard->Undisplay();
		m_keyHelp_Dive->Undisplay();
		m_keyHelp_Target->Undisplay();
	}
	//ターゲッティングしている
	if (GameManager::GetInstance().m_camera->IsTargettingEnemy())
	{
		//m_keyHelp_Guard->SetActive(true);
		//m_keyHelp_Dive->SetActive(true);

		m_keyHelp_Guard->Display();
		m_keyHelp_Dive->Display();
		m_keyHelp_Run->SetPos({ 200.f,-100.f - space,0.f });
	}
	//近くに敵がいる
	else if(EnemyManager::GetEnemyManager().GetTargettingEnemy() == nullptr)
	{
		//m_keyHelp_Target->SetActive(true);
		m_keyHelp_Target->Display();
		m_keyHelp_Run->SetPos({ 200.f,-100.f - space*2.f,0.f });
	}


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
	auto& preset = p->GetCurrentAttackPreset();
	if (preset == Player::EnAttackPreset::enRemoteAttackPreset and g_pad[0]->IsPress(EnButton::enButtonB)) {
		m_hipShot.GenerateProjectiles(p);
	}
	else {
		if (g_pad[0]->IsTrigger(EnButton::enButtonB) or g_pad[0]->IsTrigger(EnButton::enButtonY)) {
			auto nextState = p->GetState(Player::EnState::enAttack);
			return nextState;
		}
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

	//stop?
	if (p->GetVelocity().Length() < 0.1) {
		//m_keyHelp_Run->SetActive(false);
		m_keyHelp_Run->Undisplay();
	}
	else {
		if (!m_keyHelp_Run->IsActive())
			m_keyHelp_Run->Display();
		//m_keyHelp_Run->SetActive(true);
		const float space = 25.f;
		if (isEnemyCamera)
		{
			m_keyHelp_Run->SetPos({ 200.f,-100.f - space * 2.f ,0 });
		}
		else if (EnemyManager::GetEnemyManager().GetTargettingEnemy() != nullptr)
		{
			m_keyHelp_Run->SetPos({ 200.f,-100.f - space ,0 });
		}
		else
		{
			m_keyHelp_Run->SetPos({ 200.f,-100.f,0 });
		}
	}

	//近くに敵がいる
	if (EnemyManager::GetEnemyManager().GetTargettingEnemy() == nullptr)
	{
		//m_keyHelp_Target->SetActive(false);
		if (m_keyHelp_Target->IsActive())
		{
			m_keyHelp_Target->Undisplay();
		}
	}
	else
	{
		
		//m_keyHelp_Target->SetActive(true);
		if (!isEnemyCamera &&!m_keyHelp_Target->IsActive())
		{
			m_keyHelp_Target->Display();
		}
		
	}
	//ターゲッティングしている
	if (isEnemyCamera)
	{
		//m_keyHelp_Guard->SetActive(true);
		//m_keyHelp_Dive->SetActive(true);
		//m_keyHelp_Target->SetActive(false);
		if (!m_keyHelp_Guard->IsActive())
		{
			m_keyHelp_Guard->Display();
			m_keyHelp_Dive->Display();
			m_keyHelp_Target->Undisplay();
		}
	}
	else
	{
		//m_keyHelp_Guard->SetActive(false);
		//m_keyHelp_Dive->SetActive(false);
		if (m_keyHelp_Guard->IsActive())
		{
			m_keyHelp_Guard->Undisplay();
			m_keyHelp_Dive->Undisplay();
		}
	}

	return this;
}

void PlayerGroundState::Exit(Player* p) {
#ifdef _PRINT_PLAYER_STATE
	DebugPrint_WATA("Exit Ground\n");
#endif

	p->GetModelRender().SetAnimPlaySpeed(m_DefaultAnimSpeed);
	//m_keyHelp_Run->SetActive(false);
	//m_keyHelp_Guard->SetActive(false);
	//m_keyHelp_Dive->SetActive(false);

	m_keyHelp_Run->Undisplay();
	m_keyHelp_Target->Undisplay();
	m_keyHelp_Guard->Undisplay();
	m_keyHelp_Dive->Undisplay();
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