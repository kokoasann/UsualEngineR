#include "stdafx.h"
#include <algorithm>
#include "Player.h"
#include "State/PlayerFlyingState.h"
#include "State/PlayerGroundState.h"
#include "State/PlayerDiveState.h"
#include "State/PlayerAttackState.h"

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
}

void Player::OnDestroy()
{
	Release();
}


void Player::Awake()
{

	ModelInitData mid;
	mid.m_fxFilePath = "Assets/shader/AnimModel.fx";
	mid.m_vsEntryPointFunc = "VSMain";
	mid.m_psEntryPointFunc = "PSMain";
	mid.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	mid.m_tksFilePath = "Assets/modelData/unityChan.tks";
	mid.m_upAxis = enUpAxisY;

	m_model = NewGO<ModelRender>(0);
	m_model->Init(mid);
	m_model->SetScale(Vector3::One * m_scale);

	static const int numAnim = 2;
	m_animlist.resize(numAnim);
	m_animlist[static_cast<int>(EnAnimation::enRun)] = std::make_unique<CAnimationClip>();
	m_animlist[static_cast<int>(EnAnimation::enRun)]->Load("Assets/anim/unityChan/run.tka");
	m_animlist[static_cast<int>(EnAnimation::enRun)]->BuildKeyFramesAndAnimationEvents();
	m_animlist[static_cast<int>(EnAnimation::enRun)]->SetLoopFlag(true);

	m_animlist[static_cast<int>(EnAnimation::enAttack)] = std::make_unique<CAnimationClip>();
	m_animlist[static_cast<int>(EnAnimation::enAttack)]->Load("Assets/anim/unityChan/jump.tka");
	m_animlist[static_cast<int>(EnAnimation::enAttack)]->BuildKeyFramesAndAnimationEvents();
	m_animlist[static_cast<int>(EnAnimation::enAttack)]->SetLoopFlag(false);

	m_model->InitAnimation(m_animlist, m_animlist.size());
	m_model->Play(0);
}

bool Player::Start()
{

	m_stateList.resize(static_cast<int>(EnState::enNumState));
	m_stateList[static_cast<int>(EnState::enGround)] = new PlayerGroundState();
	m_stateList[static_cast<int>(EnState::enFlying)] = new PlayerFlyingState();
	m_stateList[static_cast<int>(EnState::enDiving)] = new PlayerDiveState();
	m_stateList[static_cast<int>(EnState::enAttack)] = new PlayerAttackState();

	m_currentState = m_nextState = m_stateList[static_cast<int>(EnState::enFlying)];
	m_nextState->Enter(this);

	m_charaCon.Init(m_charaConRadius, m_charaConHeight, m_position, /*isUseRigidBody */ false);

	return true;
}


void Player::PreUpdate()
{
	m_charaCon.Execute(gameTime()->GetDeltaTime(), m_velocity);
	m_position = m_charaCon.GetPosition();
	m_model->SetPosition(m_position);
	m_model->SetRotation(m_rotation);
}

void Player::Update()
{
	if (m_HP <= 0) {

	}

	m_nextState = m_currentState->Update(this);
	if (m_nextState != m_currentState) {
		m_currentState->Exit(this);
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

