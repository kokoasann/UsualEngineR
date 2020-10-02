#include "stdafx.h"
#include "PlayerFlyingState.h"
#include "Player.h"

PlayerFlyingState::PlayerFlyingState()
{

}

PlayerFlyingState::~PlayerFlyingState()
{

}

void PlayerFlyingState::Enter(Player* p) {
	printf("Enter Flying\n");

	m_bPos = p->GetPosition();
	m_vecVelocityGoal.y = 100 * m_VELOCITY_MAX;

}

IPlayerState*  PlayerFlyingState::Update(Player* p) {

	if (p->GetPosition().y > m_bPos.y + m_height) {
		m_vecVelocityGoal.y = 0.f;
	}

	auto delta = gameTime()->GetDeltaTime();
	m_velocity.x = Approach(m_vecVelocityGoal.x, m_velocity.x, delta * m_QUICKNESS);
	m_velocity.z = Approach(m_vecVelocityGoal.z, m_velocity.z, delta * m_QUICKNESS);
	m_velocity.y = Approach(m_vecVelocityGoal.y, m_velocity.y, delta * m_QUICKNESS);

	auto vel = m_velocity;
	vel *= p->GetSpeed() * gameTime()->GetDeltaTime();
	p->SetVelocity(vel);

	if (g_pad[0]->IsTrigger(EnButton::enButtonA)) {
		auto nextState = p->GetState(Player::EnState::enGround);
		return nextState;
	}
	return this;
}

void PlayerFlyingState::Exit(Player* p) {
	printf("Exit Flying\n");
}