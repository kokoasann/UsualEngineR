#include "stdafx.h"
#include "Boss_FatmanChargeBeamState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Boss/Boss_Fatman.h"
#include "Effect/Beam.h"

static int s = 0;
Boss_FatmanChargeBeamState::Boss_FatmanChargeBeamState()
{
	DebugPrintValue(EDebugConsoloUser::COMMON, "SSS", --s);
	//���̐������r�[���𐶐�����B
	for (int i = 0; i < IKNum; i++) {
		Beam* beam = NewGO<Beam>(0);
		BeamEffectInitData bid;
		beam->Init(bid);
		beam->SetSca(Vector3::One * 0.03);
		m_beams.push_back(beam);
	}
	/*m_beamSE = NewGO<CSoundSource>(0, "sound");
	m_beamSE->Init(L"Assets/sound/chara/beam.wav");*/
}

Boss_FatmanChargeBeamState::~Boss_FatmanChargeBeamState()
{
	DebugPrintValue(EDebugConsoloUser::COMMON, "SSS", ++s);
	//���������������폜�B
	for (int i = 0; i < m_beams.size(); i++) {
		DeleteGO(m_beams.at(i));
	}
	//DeleteGO(m_beamSE);
}

void Boss_FatmanChargeBeamState::Enter(IEnemy* e)
{
	e->GetModel()->SetRotation(Boss_Fatman::EnemyToPlayerRotation(e));

	//�_���[�W�����v�Z�B
	const float time = 10.f;
	m_damage = Boss_Fatman::CalcDamage(time);

	//�^�C�}�[�̃��Z�b�g�B
	m_chargeTimer = 0.f;
	m_endBeamTimer = 0.f;

	m_isSetPos = false;

	//IK���B
	for (int i = 0; i < IKNum; i++){
		m_ik[i] = e->GetIK(TO_INT(IEnemy::EnIK::enArm_L) + i);
	}

	CSoundSource* se = NewGO<CSoundSource>(0);
	se->Init(L"Assets/sound/boss_fatman/charge.wav");
	se->Play(false);
}

IEnemyState* Boss_FatmanChargeBeamState::Update(IEnemy* e)
{
	if (Charge(e)) {
		m_endBeamTimer += gameTime()->GetDeltaTime();
		const float endTime = 2.f;		//�����Ă��鎞�ԁB
		if (m_endBeamTimer < endTime) {
			//�r�[������������ݒ�B
			if (!m_isSetPos) {
				auto& p = GameManager::GetInstance().m_player;
				m_position = p->GetPosition();
				m_isSetPos = true;
				m_position.y += 4.f;
			}
			//�r�[����2�{����̂�2�񔻒���s���B
			for (int i = 0; i < IKNum; i++) {
				//�U������B
				if (BeamJudge(e,i)) {
					//�v���C���[�����ł����猂�����Ƃ��B
					auto& p = GameManager::GetInstance().m_player;
					if (!m_isKnockBack && !p->IsOnGround()){
						p->ApplyDamage(m_damage, true, Vector3::Zero);
						m_isKnockBack = true;
					}
					else {
						p->ApplyDamage(m_damage);
					}
					m_endBeamTimer = 0.f;
				}
			}
		}
		else {
			m_isKnockBack = false;
			m_isBeamSound = false;
			//m_beamSE->Stop();
			return e->GetState(TO_INT(IEnemy::EnState::enStunState));
		}
	}
	return this;
}

void Boss_FatmanChargeBeamState::Exit(IEnemy* e)
{
}

bool Boss_FatmanChargeBeamState::Charge(IEnemy* e)
{
	float chargeTime = 3.0f;		//���߂Ă��鎞�ԁB
	if (EnBattlePhase::Mad == Boss_Fatman::GetCurrentBattlePhase()) {
		chargeTime = 1.0f;
	}
	else if (EnBattlePhase::Tired == Boss_Fatman::GetCurrentBattlePhase()) {
		chargeTime = 2.0f;
	}

	m_chargeTimer += gameTime()->GetDeltaTime();
	if (m_chargeTimer > chargeTime) {
		return true;
	}

	for (int i = 0; i < IKNum; i++) {
		//�G�t�F�N�g�̐ݒ�B
		//�G�t�F�N�g�𗭂ߏ�Ԃɐݒ�B
		m_beams[i]->SetChange(true);

		//�傫���������߂ɐݒ�B
		m_beams[i]->SetSca(Vector3::One * 0.03);

		//���̍��W�̎擾�B
		auto epos = m_ik[i]->GetEffectorBone()->GetWorldMatrix().GetTransrate();
		m_beams[i]->SetPos(epos);

		//���f������v���C���[�܂ł̕�����ݒ�B
		auto& p = GameManager::GetInstance().m_player;
		auto ppos = p->GetPosition();
		
		//�_����������ɂ���B
		const float yUp = 4.f;
		ppos.y += yUp;

		//�G�t�F�N�g�̍Đ��B
		m_beams[i]->Play();		
		m_beams[i]->SetRot(m_ik[i]->GetEffectorBone()->GetWorldMatrix().GetRotate());		

		//IK�ŖC�g�𓮂����B
		m_ik[i]->SetNextTarget(ppos);
	}
	
	//���f�����v���C���[�̕����������悤�ɐݒ�B
	e->GetModel()->SetRotation(Boss_Fatman::EnemyToPlayerRotation(e));	
	return false;
}

bool Boss_FatmanChargeBeamState::BeamJudge(IEnemy* e, int ikNo)
{
	//�G�t�F�N�g�̗��ߏ�Ԃ������B
	m_beams[ikNo]->SetChange(false);
	//�傫����傫�߂ɐݒ�B
	m_beams[ikNo]->SetSca(Vector3::One * 0.3);

	//�r�[���̕��̔���B
	//�G����v���C���[�Ɍ������x�N�g���B
	//m_position�͍ŏ��Ƀ��b�N�I�������Ƃ��̃v���C���[�̈ʒu�B
	auto epos = m_ik[ikNo]->GetEffectorBone()->GetWorldMatrix().GetTransrate();

	Vector3 vecEtoP = m_position - epos;
	//m_beams[boneNo]->SetToPlayerDir(vecEtoP);

	//�O�ρB�������ɐL�т��AVecPtoE�ɒ��s����x�N�g���B
	Vector3 EWidth;
	EWidth.Cross(vecEtoP, Vector3::Up);
	EWidth.Normalize();

	//�O�ρB�c�����ɐL�т��AVecPtoE�ɒ��s����x�N�g���B
	Vector3 EHeight;
	EHeight.Cross(vecEtoP, Vector3::Right);
	EHeight.Normalize();

	//ppos�͌��݂̃v���C���[�̈ʒu�B
	auto& p = GameManager::GetInstance().m_player;
	auto& ppos = p->GetPosition();
	Vector3 vecEtoCurrentP = ppos - epos;

	//�v���C���[�ƓG�����ɕ��ׂ��Ɖ��肵���Ƃ��̋����B
	float dirW = EWidth.Dot(vecEtoCurrentP);

	//�v���C���[�ƓG���c�ɕ��ׂ��Ɖ��肵���Ƃ��̋����B
	float dirH = EHeight.Dot(vecEtoCurrentP);

	//���ʂɂ��邩�ǂ������肷�邽�߂̓��ρB
	//�}�C�i�X����������B
	float front = vecEtoP.Dot(vecEtoCurrentP);

	//IK�ŖC�g�𓮂����B
	/*const float yUp = 4.f;
	m_position.y += yUp;*/
	m_ik[ikNo]->SetNextTarget(m_position);
	
	m_beams[ikNo]->SetPos(epos);
	m_beams[ikNo]->SetRot(m_ik[ikNo]->GetEffectorBone()->GetWorldMatrix().GetRotate());
	m_beams[ikNo]->Play();

	/*if (!m_beamSE->IsPlaying()) {
		m_beamSE->Play(true);
	}*/

	if (!m_isBeamSound) {
		m_isBeamSound = true;
		CSoundSource* se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/chara/beam.wav");
		se->Play(false);
	}

	const float beamWidth = 8.0f;		//�r�[���̕��B
	if (std::abs(dirW) < beamWidth and std::abs(dirH) < beamWidth and front > 0) {
		return true;
	}
	return false;
}
