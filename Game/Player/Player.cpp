#include "stdafx.h"
#include <algorithm>
#include "Player.h"
#include "State/PlayerFlyingState.h"
#include "State/PlayerGroundState.h"
#include "State/PlayerDiveState.h"
#include "State/PlayerAttackState.h"
#include "State/PlayerGuardState.h"
#include "State/PlayerDeadState.h"
#include "../Enemy/EnemyManager.h"
#include "../Enemy/IEnemy.h"
#include "Pod/Pod.h"
#include "../GameManager.h"
#include "../GameSceneMenu.h"
#include "Attachment/JetPack.h"

Player::Player()
{
}

Player::~Player()
{
}



void Player::Release()
{
	std::for_each(m_stateList.begin(), m_stateList.end(), [](IPlayerState* state) { delete state; });
	DeleteGO(m_model);
	DeleteGO(m_pod);
	DeleteGO(m_jetPack);
}

void Player::OnDestroy()
{
	Release();
}


void Player::Awake()
{

	ModelInitData mid;
	mid.m_tkmFilePath = "Assets/modelData/m/m_ExBone.tkm";
	mid.m_tksFilePath = "Assets/modelData/m/m_ExBone.tks";
	mid.m_vsfxFilePath = "Assets/shader/AnimModel.fx";
	mid.m_upAxis = EUpAxis::enUpAxisY;
	m_model = NewGO<ModelRender>(0);
	m_model->Init(mid);
	m_model->SetScale(m_scale);
	//m_model->SetMulColor({ 0.5, 0.5, 0.4, 1.f });

	//
	/*
	ModelInitData mid;
	mid.m_vsfxFilePath = "Assets/shader/AnimModel.fx";
	mid.m_vsEntryPointFunc = "VSMain";
	mid.m_psEntryPointFunc = "PSMain";
	mid.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	mid.m_tksFilePath = "Assets/modelData/unityChan.tks";
	mid.m_upAxis = enUpAxisY;

	m_model = NewGO<ModelRender>(0);
	m_model->Init(mid);
	m_model->SetScale(Vector3::One * m_scale);
	*/

	//Init Animation
	const int numAnim = static_cast<int>(EnAnimation::enNumAnim);
	m_animlist.resize(numAnim);

	//Idle
	m_animlist[static_cast<int>(EnAnimation::enIdle)] = std::make_unique<CAnimationClip>();
	m_animlist[static_cast<int>(EnAnimation::enIdle)]->Load("Assets/modelData/m/anim/m_idle.tka");
	m_animlist[static_cast<int>(EnAnimation::enIdle)]->BuildKeyFramesAndAnimationEvents();
	m_animlist[static_cast<int>(EnAnimation::enIdle)]->SetLoopFlag(true);

	//Run
	m_animlist[static_cast<int>(EnAnimation::enRun)] = std::make_unique<CAnimationClip>();
	m_animlist[static_cast<int>(EnAnimation::enRun)]->Load("Assets/modelData/m/anim/m_dash.tka");
	m_animlist[static_cast<int>(EnAnimation::enRun)]->BuildKeyFramesAndAnimationEvents();
	m_animlist[static_cast<int>(EnAnimation::enRun)]->SetLoopFlag(true);

	//Walk
	m_animlist[static_cast<int>(EnAnimation::enWalk)] = std::make_unique<CAnimationClip>();
	m_animlist[static_cast<int>(EnAnimation::enWalk)]->Load("Assets/modelData/m/anim/m_walk.tka");
	m_animlist[static_cast<int>(EnAnimation::enWalk)]->BuildKeyFramesAndAnimationEvents();
	m_animlist[static_cast<int>(EnAnimation::enWalk)]->SetLoopFlag(true);

	//TODO : load got damage anim
	//Damage
	m_animlist[static_cast<int>(EnAnimation::enDamage)] = std::make_unique<CAnimationClip>();
	m_animlist[static_cast<int>(EnAnimation::enDamage)]->Load("Assets/modelData/m/anim/m_idle.tka");
	m_animlist[static_cast<int>(EnAnimation::enDamage)]->BuildKeyFramesAndAnimationEvents();
	m_animlist[static_cast<int>(EnAnimation::enDamage)]->SetLoopFlag(false);

	//Attack
	m_animlist[static_cast<int>(EnAnimation::enAttack)] = std::make_unique<CAnimationClip>();
	m_animlist[static_cast<int>(EnAnimation::enAttack)]->Load("Assets/modelData/m/anim/m_punch_1.tka");
	m_animlist[static_cast<int>(EnAnimation::enAttack)]->BuildKeyFramesAndAnimationEvents();
	m_animlist[static_cast<int>(EnAnimation::enAttack)]->SetLoopFlag(false);

	//Punch1
	m_animlist[static_cast<int>(EnAnimation::enPunch1)] = std::make_unique<CAnimationClip>();
	m_animlist[static_cast<int>(EnAnimation::enPunch1)]->Load("Assets/modelData/m/anim/m_punch_1.tka");
	m_animlist[static_cast<int>(EnAnimation::enPunch1)]->BuildKeyFramesAndAnimationEvents();
	m_animlist[static_cast<int>(EnAnimation::enPunch1)]->SetLoopFlag(false);

	//Punch2
	m_animlist[static_cast<int>(EnAnimation::enPunch2)] = std::make_unique<CAnimationClip>();
	m_animlist[static_cast<int>(EnAnimation::enPunch2)]->Load("Assets/modelData/m/anim/m_punch_2.tka");
	m_animlist[static_cast<int>(EnAnimation::enPunch2)]->BuildKeyFramesAndAnimationEvents();
	m_animlist[static_cast<int>(EnAnimation::enPunch2)]->SetLoopFlag(false);

	//Punch3
	m_animlist[static_cast<int>(EnAnimation::enPunch3)] = std::make_unique<CAnimationClip>();
	m_animlist[static_cast<int>(EnAnimation::enPunch3)]->Load("Assets/modelData/m/anim/m_punch_3.tka");
	m_animlist[static_cast<int>(EnAnimation::enPunch3)]->BuildKeyFramesAndAnimationEvents();
	m_animlist[static_cast<int>(EnAnimation::enPunch3)]->SetLoopFlag(false);

	//Kick
	m_animlist[static_cast<int>(EnAnimation::enKick)] = std::make_unique<CAnimationClip>();
	m_animlist[static_cast<int>(EnAnimation::enKick)]->Load("Assets/modelData/m/anim/m_punch_3.tka");
	m_animlist[static_cast<int>(EnAnimation::enKick)]->BuildKeyFramesAndAnimationEvents();
	m_animlist[static_cast<int>(EnAnimation::enKick)]->SetLoopFlag(false);

	//TODO : load dead anim
	//Dead
	m_animlist[static_cast<int>(EnAnimation::enDead)] = std::make_unique<CAnimationClip>();
	m_animlist[static_cast<int>(EnAnimation::enDead)]->Load("Assets/modelData/m/anim/m_idle.tka");
	m_animlist[static_cast<int>(EnAnimation::enDead)]->BuildKeyFramesAndAnimationEvents();
	m_animlist[static_cast<int>(EnAnimation::enDead)]->SetLoopFlag(false);

	m_model->InitAnimation(m_animlist, m_animlist.size());
	m_model->Play(0);
}

bool Player::Start()
{
	//State
	m_stateList.resize(static_cast<int>(EnState::enNumState));
	m_stateList[static_cast<int>(EnState::enGround)] = new PlayerGroundState();
	m_stateList[static_cast<int>(EnState::enFlying)] = new PlayerFlyingState();
	m_stateList[static_cast<int>(EnState::enDiving)] = new PlayerDiveState();
	m_stateList[static_cast<int>(EnState::enAttack)] = new PlayerAttackState();
	m_stateList[static_cast<int>(EnState::enDead)] = new PlayerDeadState();
	m_stateList[static_cast<int>(EnState::enGuard)] = new PlayerGuardState();

	m_currentState = m_nextState = m_stateList[static_cast<int>(EnState::enFlying)];
	m_nextState->Enter(this);

	//Physics
	m_charaCon.Init(m_charaConRadius, m_charaConHeight, m_position, /*isUseRigidBody */ true);

	//Pod
	m_pod = NewGO<Pod>(0);
	m_pod->SetPlayer(this);

	//Attachment
	m_playerBones.resize(TO_INT(EnPlayerBone::enNumBoneType));
	m_jetPack = NewGO<JetPack>(0);
	m_jetPack->SetActive(false);

	auto bone = this->GetModel().GetSkelton()->GetBone(this->GetModel().GetSkelton()->FindBoneID(L"Bone.005"));
	m_playerBones.at(TO_INT(EnPlayerBone::enBack)) = bone;

	return true;
}


void Player::PreUpdate()
{
	if (GameManager::GetInstance().m_menu->IsGamePaused()) return;

	m_charaCon.Execute(gameTime()->GetDeltaTime(), m_velocity);
	m_position = m_charaCon.GetPosition();
	m_model->SetPosition(m_position);
	m_model->SetRotation(m_rotation);
}

void Player::Update()
{
	if (GameManager::GetInstance().m_menu->IsGamePaused()) return;

	//Attachments
	//Jetpack
	if(m_currentAttackPreset == EnAttackPreset::enMeleePreset){
		m_jetPack->SetActive(true);
		const auto& mat = m_playerBones.at(TO_INT(EnPlayerBone::enBack))->GetWorldMatrix();
		m_jetPack->SetPosition(mat.GetTransrate());
		m_jetPack->SetRotation(mat.GetRotate());
	}
	else {
		m_jetPack->SetActive(false);
	}

	SearchTarget();
	UpdateAttackType();

	//Calc forward
	auto& world = m_model->GetModel().GetWorldMatrix();
	m_forward.x = world.mat._31;
	m_forward.y = world.mat._32;
	m_forward.z = world.mat._33;
	m_forward.Normalize();

	m_nextState = m_currentState->Update(this);

	if (m_hp <= 0) {
		m_nextState = m_stateList[static_cast<int>(EnState::enDead)];
	}

	if (m_nextState != m_currentState) {
		m_currentState->Exit(this);
		m_previousState = m_currentState;
		m_currentState = m_nextState;
		m_currentState->Enter(this);
	}
}

void Player::PostUpdate()
{

}



void Player::Render()
{

}

void Player::PostRender()
{

}

void Player::SearchTarget() {
	auto enemy = EnemyManager::GetEnemyManager().GetNearestEnemy();
	if (enemy != nullptr /*and enemy->IsBoss()*/ and (enemy->GetState(IEnemy::EnState::enIdleState) != enemy->GetCurrentState() or
		enemy->GetState(IEnemy::EnState::enDeadState) != enemy->GetCurrentState())) {
		m_target = enemy;
	}
	else {
		m_target = nullptr;
	}
}

void Player::UpdateAttackType() {

	if (g_pad[0]->IsTrigger(enButtonLeft)) {
		if (m_canUsePreset[TO_INT(EnAttackPreset::enRemoteAttackPreset)]) {
			m_currentAttackPreset = EnAttackPreset::enRemoteAttackPreset;
		}
	}

	if (g_pad[0]->IsTrigger(enButtonUp)) {
		if (m_canUsePreset[TO_INT(EnAttackPreset::enMeleePreset)])
			m_currentAttackPreset = EnAttackPreset::enMeleePreset;
	}

	if (g_pad[0]->IsTrigger(enButtonRight)) {
		if (m_canUsePreset[TO_INT(EnAttackPreset::enExposivePreset)])
			m_currentAttackPreset = EnAttackPreset::enExposivePreset;
	}

	if (g_pad[0]->IsTrigger(enButtonDown)) {
		m_currentAttackPreset = EnAttackPreset::enDefault;
	}
}