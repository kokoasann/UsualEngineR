#include "stdafx.h"
#include "PlayerAttackState.h"
#include "../Player.h"
#include "../Attack/IPlayerAttack.h"
#include "../Attack/PlayerAttackSlash.h"

PlayerAttackState::PlayerAttackState() {
}

PlayerAttackState::~PlayerAttackState() {

}

void PlayerAttackState::Enter(Player* p) {
	printf("Attack Enter\n");
	auto& vel = p->GetLocalVelocity();
	p->SetVelocity(Vector3::Zero);

	m_combo = 0;

	//‚±‚Ì•Ó‚Å‰½‚ÌUŒ‚‚ª‚Å‚«‚é‚©”»’f‚·‚é
	//m_currentAttack = p->GetNormalAttackSkill()
	m_currentAttack = new PlayerAttackSlash();

	m_currentAttack->Init(p,m_combo);
	m_currentAttack->GetInterval();
}

void PlayerAttackState::Exit(Player* p) {
	printf("Attack Exit\n");
}

IPlayerState* PlayerAttackState::Update(Player* p) {

	m_currentAttack->Execute(p);
	
	if (m_currentAttack->IsDone()) {
		if (m_elpased <= m_currentAttack->GetInterval()) {
			m_elpased += gameTime()->GetDeltaTime();
			return this;
		}
		auto ca = m_currentAttack->IsContinueAttack();
		if (ca and m_combo < m_MAX_COMBO) {
			m_combo++;
			m_currentAttack->Init(p,m_combo);
		}
		else {
			auto nextState = p->GetState(Player::EnState::enGround);
			return nextState;
		}
	}

	return this;
}