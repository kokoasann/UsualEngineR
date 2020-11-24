#include "stdafx.h"
#include "PlayerGuardState.h"
#include "../Player.h"

PlayerGuardState::PlayerGuardState() {
}

PlayerGuardState::~PlayerGuardState() {
}


void PlayerGuardState::Enter(Player* p) {

#ifdef _PRINT_PLAYER_STATE
	DebugPrint_WATA("Player : Enter Guard\n");
#endif

	p->SetVelocity(Vector3::Zero);
	p->PlayAnimation(Player::EnAnimation::enGuard);
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
	DebugPrint_WATA("Player : Exit Guard\n");
#endif
	//TODO : プレイヤーの元の防御力に戻す
	p->SetArmorParam(0.f);
}

