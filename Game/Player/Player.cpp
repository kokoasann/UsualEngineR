#include "stdafx.h"
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

}

void Player::OnDestroy()
{

}



void Player::Awake()
{

}

bool Player::Start()
{
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

