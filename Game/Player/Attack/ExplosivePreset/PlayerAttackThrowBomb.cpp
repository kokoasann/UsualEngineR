#include "stdafx.h"
#include "PlayerAttackThrowBomb.h"
#include "../../Player.h"
#include "../../../Enemy/EnemyManager.h"
#include "../Bomb.h"

PlayerAttackThrowBomb::PlayerAttackThrowBomb() {

}

PlayerAttackThrowBomb::~PlayerAttackThrowBomb() {

}

void PlayerAttackThrowBomb::Init(Player* player, int combo) {
#ifdef _PRINT_PLAYER_ATTACK
	std::string s = "attack throw bomb combo :" + std::to_string(combo);
	DebugPrint_WATA(s.c_str());
#endif
	m_isDone = false;
	m_isContinuAttack = false;
	m_timer = 0.f;

	//NOTE : ここのinterpolateを0.fにしないと、前の投げアニメーションのイベントがいきなり呼ばれる事がある.
	//連続で爆弾をなげようとボタンを連打した際にバグる.
	player->PlayAnimation(Player::EnAnimation::enThrow, 0.f);
	threwFlag = false;
	//auto& enemyManager = EnemyManager::GetEnemyManager();
	//enemyManager.ApplyAoeDamage(/*attack origin*/ player->GetPosition(), m_range, m_damageAmount * combo);

}

void PlayerAttackThrowBomb::Execute(Player* player) {
	//TODO : if(!animation.isPlay()) m_timer += deltaTime(); 
	//m_timer += gameTime()->GetDeltaTime();

	const int numEvent = player->GetAnimationMap().at(TO_INT(Player::EnAnimation::enThrow))->GetNumAnimationEvent();
	const auto& animationEvent = player->GetAnimationMap().at(TO_INT(Player::EnAnimation::enThrow))->GetAnimationEvent();

	for (int i = 0; i < numEvent; i++) {
		if (!threwFlag and animationEvent[i].IsInvoked()) {
			auto name = animationEvent[i].GetEventName();
			if (wcscmp(name, L"throw") == 0) {
				printf("throw\n");
				auto bomb = NewGO<Bomb>(0);
				auto vel = player->GetForward() * m_speed;
				bomb->Init(player->GetPosition(), vel);
				threwFlag = true;
			}
		}
	}


	if (!player->IsPlayingAnimation()) {
		m_timer += gameTime()->GetDeltaTime();
	}

	if (m_timer >= m_interval) {
		m_isDone = true;
	}
}


