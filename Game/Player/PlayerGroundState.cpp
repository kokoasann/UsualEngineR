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
	printf("Ground Enter\n");
}

IPlayerState* PlayerGroundState::Update(Player* p) {
	//Move
	auto lxf = g_pad[0]->GetLStickXF();
	auto lyf = g_pad[0]->GetLStickYF();

	m_vecVelocityGoal.x = lxf * 20;
	m_vecVelocityGoal.z = lyf * 20;

	if (g_pad[0]->IsPress(enButtonA)) {
		m_vecVelocityGoal.x = 20;
	}
	if (g_pad[0]->IsPress(enButtonB)) {
		m_vecVelocityGoal.x = -20;
	}

	auto delta = gameTime()->GetDeltaTime();

	m_velocity.x = Approach(m_vecVelocityGoal.x, m_velocity.x, delta * 20);
	printf("Velocity X : %f\n", m_velocity.x);
	auto nextPos = p->GetPosition() + m_velocity * gameTime()->GetDeltaTime();
	p->SetPosition(nextPos);
	
	/*
	if (g_pad[0]->IsTrigger(EnButton::enButtonA)) {
		 auto nextState =  p->GetState(Player::EnState::enFlying);
		return nextState;
	}
	*/
	return this;
}

void PlayerGroundState::Exit(Player* p) {
	printf("Ground Exit\n");
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


Vector3 PlayerGroundState::Approach(Vector3 goal, Vector3 current, float dt) {
	Vector3 ret = goal;
	auto diff = current - goal;
	if (diff.x > dt) {
		ret.x += dt;
	}
	else if (diff.x < dt) {
		ret.x -= dt;
	}
	if (diff.y > dt) {
		ret.y += dt;
	}
	else if (diff.y < dt) {
		ret.y -= dt;
	}
	if (diff.z > dt) {
		diff.z += dt;
	}
	else if (diff.z < dt) {
		diff.z -= dt;
	}
	return ret;
}