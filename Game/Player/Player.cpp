#include "stdafx.h"
#include "Player.h"
#include "IPlayerSstate.h"

static PlayerGroundState m_playerGroundState;

Player::Player()
{

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
	m_playerState = &m_playerGroundState;
	return true;
}



void Player::PreUpdate()
{

}

void Player::Update()
{
	auto state = m_playerState->Update();
	if (state != m_currentState) {
		state->Exit();
		m_currentState = state;
		state->Enter();
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
