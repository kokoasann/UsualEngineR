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
	m_gravity = 9.f;
	m_respawnTimer = 0.f;

	for (int i = 0; i < TO_INT(Player::EnPlayerBone::enNumBoneType); i++) {
		auto bonePos = p->GetIK(i)->GetEffectorBone()->GetWorldMatrix().GetTransrate();
		p->GetIK(i)->SetIKMode(IK::IKMode::enMode_NoAnimHit);
		bonePos.y += 1.f;
		p->GetIK(i)->SetNextTarget(bonePos);
	}

	m_velocity = p->m_knockBackImpulse;
	m_fricVel = m_velocity;
	m_fricVel.y = 0;
	m_fricVel.Normalize();
	p->m_knockBackImpulse = Vector3::Zero;
}

void PlayerDeadState::Exit(Player* p) {
#ifdef _PRINT_PLAYER_STATE
	DebugPrint_WATA("Player - Exit Dead State\n");
#endif

	for (int i = 0; i < TO_INT(Player::EnPlayerBone::enNumBoneType); i++) {
		auto bonePos = p->GetIK(i)->GetEffectorBone()->GetWorldMatrix().GetTransrate();
		p->GetIK(i)->SetIKMode(IK::IKMode::enMode_NoneHit);
	}

	p->SetVelocity(Vector3::Zero);
	p->SetLocalVelocity(Vector3::Zero);
	p->SetExternalVelocity(Vector3::Zero);
}

IPlayerState* PlayerDeadState::Update(Player* p) {

	auto delta = gameTime()->GetDeltaTime();
	m_respawnTimer += delta;

	m_velocity.y -= m_gravity * delta;
	if (p->IsOnGround())
	{
		const float fricPow = 10;
		auto veloXZ = m_velocity;
		if (veloXZ.Length() > fricPow)
		{
			m_velocity -= m_fricVel * fricPow;
		}
		else
		{
			m_velocity.x = 0;
			m_velocity.z = 0;
		}
	}

	for (int i = 0; i < TO_INT(Player::EnPlayerBone::enNumBoneType); i++) {
		auto bonePos = p->GetIK(i)->GetEffectorBone()->GetWorldMatrix().GetTransrate() + m_velocity;
		//bonePos.y -= m_gravity * gameTime()->GetDeltaTime();
		if (!p->GetIK(i)->IsHit()) {
			p->GetIK(i)->SetNextTarget(bonePos);
		}
	}

	static const float GravityAddVal = 30.f;
	m_gravity += GravityAddVal * delta;

	auto plpos = p->GetPosition();
	plpos.y -= 9 * delta;
	//p->SetVelocity(plpos);
	//p->SetPosition(plpos);

	p->SetVelocity(m_velocity);

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