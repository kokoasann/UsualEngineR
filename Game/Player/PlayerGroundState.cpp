#include "stdafx.h"
#include "PlayerGroundState.h"
#include <cmath>

PlayerGroundState::PlayerGroundState()
{
}

PlayerGroundState::~PlayerGroundState()
{
}

void PlayerGroundState::Enter(Player* p){
	printf("Enter Ground\n");
}

IPlayerState* PlayerGroundState::Update(Player* p) {
	//Move
	auto lxf = g_pad[0]->GetLStickXF();
	auto lyf = g_pad[0]->GetLStickYF();

	m_vecVelocityGoal.x = lxf * -m_VELOCITY_MAX;
	m_vecVelocityGoal.z = lyf * -m_VELOCITY_MAX;

	if (g_pad[0]->IsPress(enButtonLeft)) {
		m_vecVelocityGoal.x = m_VELOCITY_MAX;
	}
	if (g_pad[0]->IsPress(enButtonRight)) {
		m_vecVelocityGoal.x = -m_VELOCITY_MAX;
	}

	auto delta = gameTime()->GetDeltaTime();

	m_velocity.x = Approach(m_vecVelocityGoal.x, m_velocity.x, delta * m_QUICKNESS);
	m_velocity.z = Approach(m_vecVelocityGoal.z, m_velocity.z, delta * m_QUICKNESS);

	auto vel = m_velocity * gameTime()->GetDeltaTime() * p->GetSpeed();
	p->SetVelocity(vel);
	
	if (g_pad[0]->IsTrigger(EnButton::enButtonA)) {
		 auto nextState =  p->GetState(Player::EnState::enFlying);
		return nextState;
	}

	return this;
}

void PlayerGroundState::Exit(Player* p) {
	printf("Exit Ground\n");
}
