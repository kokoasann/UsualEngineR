#include "stdafx.h"
#include "PlayerDeadState.h"
#include "Fade.h"
#include "../Player.h"
#include "../../GameManager.h"
#include "../../Camera/GameCamera.h"

PlayerDeadState::PlayerDeadState() {
}

PlayerDeadState::~PlayerDeadState() {

}

void PlayerDeadState::Enter(Player* p) {
#ifdef _PRINT_PLAYER_STATE
	DebugPrint_WATA("Player - Enter Dead State\n");
#endif
	auto& vel = p->GetLocalVelocity();
	p->SetVelocity(Vector3::Zero);
	p->SetLocalVelocity(Vector3::Zero);
	p->SetExternalVelocity(Vector3::Zero);
	p->PlayAnimation(Player::EnAnimation::enDead);
	p->Explode();
	auto explodeSe = NewGO<CSoundSource>(0);
	explodeSe->Init(L"Assets/sound/chara/explosion.wav");
	explodeSe->Play(false);
	m_isFadedToRespawn = false;
}

void PlayerDeadState::Exit(Player* p) {
	m_respawnTimer = 0.f;
#ifdef _PRINT_PLAYER_STATE
	DebugPrint_WATA("Player - Exit Dead State\n");
#endif
}

IPlayerState* PlayerDeadState::Update(Player* p) {

	auto delta = gameTime()->GetDeltaTime();
	m_respawnTimer += delta;

	if (m_respawnTimer > m_fadeOutTime and !m_isFadedToRespawn) {
		Fade::GetInstance().FadeOut();
		m_isFadedToRespawn = true;
	}

	if (m_respawnTimer > m_RespawnTime) {
		p->Respawn();
		GameManager::GetInstance().m_camera->Reset();
		auto nextState = p->GetState(Player::EnState::enFlying);
		Fade::GetInstance().FadeIn();
		return nextState;
	}

	return this;
}