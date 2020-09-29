#include "stdafx.h"
#include <algorithm>
#include "Player.h"
#include "PlayerFlyingState.h"
#include "PlayerGroundState.h"

PlayerFlyingState m_playerFlyingState;
PlayerGroundState m_playerGroundState;

Player::Player()
{
	AllocConsole();

	freopen("CON", "r", stdin);
	freopen("CON", "w", stdout);
	freopen("CON", "w", stderr);
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

}

bool Player::Start()
{

	m_stateList.resize(EnState::enNumState);
	m_stateList[enGround] = new PlayerGroundState();
	m_stateList[enFlying] = new PlayerFlyingState();

	m_currentState = m_playerState = &m_playerGroundState;
	m_playerState->Enter(this);

	return true;
}



void Player::PreUpdate()
{

}

void Player::Update()
{
	auto state = m_playerState->Update(this);
	if (state != m_currentState) {
		state->Exit(this);
		m_currentState = state;
		state->Enter(this);
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

