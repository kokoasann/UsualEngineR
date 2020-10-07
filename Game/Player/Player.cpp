#include "stdafx.h"
#include <algorithm>
#include "Player.h"
#include "PlayerFlyingState.h"
#include "PlayerGroundState.h"
#include "PlayerDiveState.h"

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
	m_animlist.resize(1);
	m_animlist[0] = std::make_unique<CAnimationClip>();
	m_animlist[0]->Load("Assets/anim/unityChan/run.tka");
	m_animlist[0]->BuildKeyFramesAndAnimationEvents();
	m_animlist[0]->SetLoopFlag(true);

	m_model->InitAnimation(m_animlist, 1);
	m_model->Play(0);
}

bool Player::Start()
{

	m_stateList.resize(EnState::enNumState);
	m_stateList[enGround] = new PlayerGroundState();
	m_stateList[enFlying] = new PlayerFlyingState();
	m_stateList[enDiving] = new PlayerDiveState();

	m_currentState = m_nextState = m_stateList[EnState::enFlying];
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
	m_nextState = m_currentState->Update(this);
	if (m_nextState != m_currentState) {
		m_currentState->Exit(this);
		m_currentState = m_nextState;
		m_currentState->Enter(this);
	}

	//printf("Vel X : %f / Vel Y : %f / Vel Z : %f\n", m_velocity.x, m_velocity.y, m_velocity.z);
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

