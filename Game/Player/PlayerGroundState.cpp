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

	m_vecVelocityGoal.x = lxf * -m_MAX_VELOCITY;
	m_vecVelocityGoal.z = lyf * -m_MAX_VELOCITY;

	/*
	if (g_pad[0]->IsPress(enButtonA)) {
		m_vecVelocityGoal.x = 20;
	}
	if (g_pad[0]->IsPress(enButtonB)) {
		m_vecVelocityGoal.x = -20;
	}
	*/

	auto delta = gameTime()->GetDeltaTime();

	m_velocity.x = Approach(m_vecVelocityGoal.x, m_velocity.x, delta * m_QUICKNESS);
	m_velocity.z = Approach(m_vecVelocityGoal.z, m_velocity.z, delta * m_QUICKNESS);

	auto nextPos = p->GetPosition() + m_velocity * gameTime()->GetDeltaTime();
	p->SetPosition(nextPos);
	
	if (g_pad[0]->IsTrigger(EnButton::enButtonA)) {
		 auto nextState =  p->GetState(Player::EnState::enFlying);
		return nextState;
	}

	return this;
}

void PlayerGroundState::Exit(Player* p) {
	printf("Exit Ground\n");
}

float PlayerGroundState::Approach(float goal, float current, float dt) {
	auto diff = goal - current;
	if (diff > dt) {
		return current + dt;
	}
	if (diff < dt) {
		return current - dt;
	}	
	return goal;
}