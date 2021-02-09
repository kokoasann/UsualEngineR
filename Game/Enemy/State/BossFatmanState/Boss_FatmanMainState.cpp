#include "stdafx.h"
#include "Boss_FatmanMainState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Boss/Boss_Fatman.h"
#include "GameManager.h"
#include "../../../Game.h"
#include "Camera/GameCamera.h"
#include "Enemy/State/BossFatmanState/Boss_FatmanAngryPerform.h"


Boss_FatmanMainState::Boss_FatmanMainState()
{
	
}

Boss_FatmanMainState::~Boss_FatmanMainState()
{
	
}

void Boss_FatmanMainState::Enter(IEnemy* e)
{
	//重力。
	Vector3 gravity = Vector3::Zero;
	gravity.y = -5000.f;
	e->SetVelocity(gravity);
	
	e->PlayAnimation(IEnemy::EnAnimation::enIdle);

	//IK情報。
	for (int i = 0; i < IK_NUM; i++) {
		m_BeamIk[i] = e->GetIK(TO_INT(IEnemy::EnIK::enArm_L) + i);
	}	
	for (int i = 0; i < IK_NUM; i++) {
		m_ShootIk[i] = e->GetIK(TO_INT(IEnemy::EnIK::enFoot_L) + i);
	}	

	//出会った時の演出。
	if (!m_isPerformed) {
		GameManager::GetInstance().m_gameScene->OnEnterBattle(e);
		m_isPerformed = true;
	}
	//怒った時の演出。
	if (!m_isAngryPerformed 
		&& EnBattlePhase::Mad == Boss_Fatman::GetCurrentBattlePhase()) {
		e->PlayAnimation(TO_INT(Boss_Fatman::EnAnimEX::enAngry));		

		auto cam = GameManager::GetInstance().m_camera;
		auto tar = e->GetPosition();
		tar.y += 20.f;

		auto eneForward = e->GetForward();
		auto camEndPos = e->GetPosition() + eneForward * 35.f;
		camEndPos.y += 20.f;
		auto sec = 1.f;
		auto interval = 1.7f;

		g_graphicsEngine->GetPostEffect().SetUseFocusBlurFrag(true);

		cam->Perform(
			camEndPos, camEndPos,
			tar, tar, sec, interval, GameCamera::State::enEnemyCamera
		);
		
		m_bap = NewGO<Boss_FatmanAngryPerform>(0);
		m_bap->SetBeamIk(m_BeamIk);
		m_bap->SetShootIk(m_ShootIk);

		m_isAngryPerformed = true;
	}
}

IEnemyState* Boss_FatmanMainState::Update(IEnemy* e)
{
	if (!m_isBmp && m_isAngryPerformed) {
		DeleteGO(m_bap);
		m_isBmp = true;
	}
		
	ChangeBattleState(e);

	m_fatTimer += gameTime()->GetDeltaTime();

	float attackSpan = 0.9f;
	if (EnBattlePhase::Mad == Boss_Fatman::GetCurrentBattlePhase()) {
		attackSpan = 0.3f;
	}
	else if (EnBattlePhase::Tired == Boss_Fatman::GetCurrentBattlePhase()) {
		attackSpan = 0.6f;
	}

	if (m_fatTimer > attackSpan) {
		auto& player = GameManager::GetInstance().m_player;
		const auto& ppos = player->GetPosition();
		//return e->GetState(TO_INT(Boss_Fatman::EnStateEX::enAttackE));
		if (player->GetCurrentHP() > 0.f) {
			auto& epos = e->GetPosition();
			auto& ppos = player->GetPosition();
			auto vecToPlayer = ppos - epos;

			float diffY = ppos.y - epos.y;
			const float flyRange = 5.f;
			const float attackRange = 75.f;

			//離れたら遠距離攻撃。
			//もしくはプレイヤーが飛んでいたら遠距離攻撃。
			if (vecToPlayer.Length() > Boss_Fatman::TAKE_DISTANCE or diffY > flyRange) {
				m_fatTimer = 0.f;
				switch (Boss_Fatman::GetCurrentBattlePhase()) {
				case EnBattlePhase::Normal:
					return LongRangeAttack(e);
					break;
				case EnBattlePhase::Mad:
				case EnBattlePhase::Tired:
					//離れたら遠距離攻撃。
					//もしくはプレイヤーが飛んでいたら遠距離攻撃。
					if (vecToPlayer.Length() > attackRange or diffY > flyRange) {
						//確率で行動変化。
						auto rand = GRandom().Rand();
						if (rand < 0.5f) {
							//ビーム。
							return e->GetState(TO_INT(IEnemy::EnState::enAttackA));
						}
						else {
							//溜めビーム＆乱射。
							return e->GetState(TO_INT(Boss_Fatman::EnStateEX::enAttackE));
						}
					}
					break;
				}
			}
			//近づいたらタックルか距離をとる。
			else {
				//確率で行動変化。
				auto rand = GRandom().Rand();
				if (rand < 0.5f) {
					//タックル。
					return e->GetState(TO_INT(Boss_Fatman::EnStateEX::enAttackD));
				}
				else {
					//距離をとる。
					return e->GetState(TO_INT(Boss_Fatman::EnStateEX::enTakeDistance));
				}
			}
		}
		else {
			return e->GetState(TO_INT(IEnemy::EnState::enIdleState));
		}
	}
	return this;
}

void Boss_FatmanMainState::Exit(IEnemy* e)
{
}

IEnemyState* Boss_FatmanMainState::LongRangeAttack(IEnemy* e)
{
	//確率で行動変化。
	auto rand = GRandom().Rand();
	if (rand < 0.33f) {
		//ビーム。
		return e->GetState(TO_INT(IEnemy::EnState::enAttackA));
	}
	else if (rand < 0.66f) {
		//溜めビーム。
		return e->GetState(TO_INT(IEnemy::EnState::enAttackB));
	}
	else {
		//乱射。
		return e->GetState(TO_INT(Boss_Fatman::EnStateEX::enAttackC));
	}
}

void Boss_FatmanMainState::ChangeBattleState(IEnemy* e)
{
	const float percentage_Tired = 2.0f / 5.0f;
	const float percentage_Mad = 2.0f / 3.0f;
	if (e->GetCurrentHP() < e->GetMaxHP() * percentage_Tired) {
		Boss_Fatman::SetBattlePhase(EnBattlePhase::Tired);
	}
	else if (e->GetCurrentHP() < e->GetMaxHP() * percentage_Mad) {
		Boss_Fatman::SetBattlePhase(EnBattlePhase::Mad);
	}
}