#include "stdafx.h"
#include "PlayerAttackState.h"
#include "../Player.h"
#include "../Attack/IPlayerAttack.h"
#include "../Attack/PlayerAttackSlash.h"
#include "../Attack/PlayerAttackA.h"

PlayerAttackState::PlayerAttackState() {
}

PlayerAttackState::~PlayerAttackState() {

}

void PlayerAttackState::Enter(Player* p) {

#ifdef _PRINT_PLAYER_STATE
	DebugPrint_WATA("Player Attack Enter\n");
#endif

	auto& vel = p->GetLocalVelocity();
	p->SetVelocity(Vector3::Zero);

	m_combo = 1;

	//‚±‚Ì•Ó‚Å‰½‚ÌUŒ‚‚ª‚Å‚«‚é‚©”»’f‚·‚é
	//m_currentAttack = p->GetNormalAttackSkill()

	//TODO : Avoid using new everytime.
	switch (p->GetNormalAttackType()) {

	case Player::EnAttackType::enSlash:
		m_currentAttack = new PlayerAttackSlash();
		break;

	case Player::EnAttackType::enA:
		m_currentAttack = new PlayerAttackA();
		break;

	default:
		m_currentAttack = new PlayerAttackSlash();

	}
	

	m_currentAttack->Init(p,m_combo);
	m_currentAttack->GetInterval();
}

void PlayerAttackState::Exit(Player* p) {
#ifdef _PRINT_PLAYER_STATE
	DebugPrint_WATA("Attack Exit\n");
#endif
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
			return p->GetPreviousState();
			//auto nextState = p->GetState(Player::EnState::enGround);
			//return nextState;
		}
	}

	return this;
}