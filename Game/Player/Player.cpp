#include "stdafx.h"
#include <algorithm>
#include "Player.h"
#include "PlayerFlyingState.h"
#include "PlayerGroundState.h"

Player::Player()
{
	/*
	AllocConsole();
	freopen("CON", "r", stdin);
	freopen("CON", "w", stdout);
	freopen("CON", "w", stderr);
	*/
}

Player::~Player()
{

}



void Player::Release()
{
	std::for_each(m_stateList.begin(), m_stateList.end(), [](IPlayerState* state) { delete state; });
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
	m_model->SetScale(g_vec3One * 0.05);
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

	m_currentState = m_nextState = m_stateList[EnState::enGround];
	m_nextState->Enter(this);

	return true;
}



void Player::PreUpdate()
{

}

void Player::Update()
{
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

