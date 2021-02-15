#include "stdafx.h"
#include "Boss_FatmanMainState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Boss/Boss_Fatman.h"
#include "GameManager.h"
#include "../../../Game.h"
#include "Camera/GameCamera.h"
#include "Enemy/State/BossFatmanState/Boss_FatmanAngryPerform.h"
#include "Enemy/BossBGM.h"


Boss_FatmanMainState::Boss_FatmanMainState()
{

}

Boss_FatmanMainState::~Boss_FatmanMainState()
{

}

void Boss_FatmanMainState::Enter(IEnemy* e)
{
	//�d�́B
	Vector3 gravity = Vector3::Zero;
	gravity.y = -5000.f;
	e->SetVelocity(gravity);

	e->PlayAnimation(IEnemy::EnAnimation::enIdle);

	//IK���B
	for (int i = 0; i < IK_NUM; i++) {
		m_BeamIk[i] = e->GetIK(TO_INT(IEnemy::EnIK::enArm_L) + i);
	}
	for (int i = 0; i < IK_NUM; i++) {
		m_ShootIk[i] = e->GetIK(TO_INT(IEnemy::EnIK::enFoot_L) + i);
	}

	//�o��������̉��o�B
	if (!m_isPerformed) {
		m_isPerformed = true;
		GameManager::GetInstance().m_gameScene->OnEnterBattle(e);

	}
	//�{�������̉��o�B
	if (!m_isAngryPerformed
		&& EnBattlePhase::Mad == Boss_Fatman::GetCurrentBattlePhase()) {
		e->PlayAnimation(TO_INT(Boss_Fatman::EnAnimEX::enAngry));

		CSoundSource* se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/Boss_Shout.wav");
		se->Play(false);
		se->SetVolume(1.0f);

		m_isAngryPerformed = true;

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
		if (player->GetCurrentHP() > 0.f) {
			const auto& ppos = player->GetPosition();
			const auto& epos = e->GetPosition();
			auto vecToPlayer = ppos - epos;

			//���̂��������ꂽ���~�B
			const float idleDist = 400.0f;
			if (vecToPlayer.Length() > idleDist) {
				return e->GetState(TO_INT(IEnemy::EnState::enIdleState));
			}

			//���ꂽ�牓�����U���B
			//�������̓v���C���[�����ł����牓�����U���B
			if (vecToPlayer.Length() > Boss_Fatman::TAKE_DISTANCE or !player->IsOnGround()) {
				m_fatTimer = 0.f;
				m_isTakeDistance = false;
				m_isTackle = false;
				switch (Boss_Fatman::GetCurrentBattlePhase()) {
				case EnBattlePhase::Normal:
					return LongRangeAttack(e);
					break;
				case EnBattlePhase::Mad:
				case EnBattlePhase::Tired:
					//�m���ōs���ω��B
					auto rand = GRandom().Rand();
					if (rand < 0.5f) {
						//�r�[���B
						return e->GetState(TO_INT(IEnemy::EnState::enAttackA));
					}
					else {
						//���߃r�[�������ˁB
						return e->GetState(TO_INT(Boss_Fatman::EnStateEX::enAttackE));
					}
					break;
				}
			}
			//�߂Â�����^�b�N�����������Ƃ�B
			else {
				if (m_isTakeDistance) {
					//�m���ōs���ω��B
					/*auto rand = GRandom().Rand();
					if (rand < 0.5f) {*/
					//�^�b�N���B
					m_isTakeDistance = false;
					m_isTackle = true;
					return e->GetState(TO_INT(Boss_Fatman::EnStateEX::enAttackD));
				}
				else {
					//�������Ƃ�B
					m_isTakeDistance = true;
					m_isTackle = false;
					return e->GetState(TO_INT(Boss_Fatman::EnStateEX::enTakeDistance));
				}
			}
		}
	}
	return this;
}

void Boss_FatmanMainState::Exit(IEnemy* e)
{
}

IEnemyState* Boss_FatmanMainState::LongRangeAttack(IEnemy* e)
{
	//�m���ōs���ω��B
	auto rand = GRandom().Rand();
	if (rand < 0.33f) {
		//�r�[���B
		return e->GetState(TO_INT(IEnemy::EnState::enAttackA));
	}
	else if (rand < 0.66f) {
		//���߃r�[���B
		return e->GetState(TO_INT(IEnemy::EnState::enAttackB));
	}
	else {
		//���ˁB
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