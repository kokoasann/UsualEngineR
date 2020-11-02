#include "stdafx.h"
#include "PlayerGuardState.h"
#include "../Player.h"

PlayerGuardState::PlayerGuardState() {
}

PlayerGuardState::~PlayerGuardState() {
}


void PlayerGuardState::Enter(Player* p) {

#ifdef _PRINT_PLAYER_STATE
	printf("Player : Enter Guard\n");
#endif

	p->SetVelocity(Vector3::Zero);
	p->PlayAnimation(Player::EnAnimation::enIdle);
	p->SetArmorParam(m_guardParam);
}

IPlayerState* PlayerGuardState::Update(Player* p) {
	if (g_pad[0]->IsPress(EnButton::enButtonLB2))
		return this;

	auto nextState = p->GetPreviousState();
	return nextState;
}

void PlayerGuardState::Exit(Player* p) {
#ifdef _PRINT_PLAYER_STATE
	printf("Player : Exit Guard\n");
#endif
	//TODO : ƒvƒŒƒCƒ„[‚ÌŒ³‚Ì–hŒä—Í‚É–ß‚·
	p->SetArmorParam(0.f);
}

