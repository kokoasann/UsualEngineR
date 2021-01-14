#include "stdafx.h"
#include "Boss_FatmanChargeBeamState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Boss/Boss_Fatman.h"
#include "Effect/Beam.h"

Boss_FatmanChargeBeamState::Boss_FatmanChargeBeamState()
{
	//���̐������r�[���𐶐�����B
	for (int i = 0; i < IKNum; i++) {
		Beam* beam = NewGO<Beam>(0);
		BeamEffectInitData bid;
		beam->Init(bid);
		beam->SetSca(Vector3::One * 0.03);
		m_beams.push_back(beam);
	}
}

Boss_FatmanChargeBeamState::~Boss_FatmanChargeBeamState()
{
	//���������������폜�B
	for (int i = 0; i < m_beams.size(), i++;) {
		DeleteGO(m_beams.at(i));
	}
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
					if (!p->IsOnGround()) {
						p->ApplyDamage(m_damage, true, Vector3::Zero);
					}
					else {
						p->ApplyDamage(m_damage);
					}
					m_endBeamTimer = 0.f;
				}
			}
		}
		else {
			return e->GetState(TO_INT(IEnemy::EnState::enBattleState));
		}
	}
	return this;
}

void Boss_FatmanChargeBeamState::Exit(IEnemy* e)
{
}

bool Boss_FatmanChargeBeamState::Charge(IEnemy* e)
{
	const float chargeTime = 5.f;		//���߂Ă��鎞�ԁB
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

		//Vector3 vecEtoP = ppos - epos;
		//m_beams[i]->SetToPlayerDir(vecEtoP);

		////�O�ρB���s�����c�̃x�N�g���B
		//Vector3 EHeight;
		//EHeight.Cross(vecEtoP, Vector3::Right);
		//EHeight.Normalize();
		//m_beams[i]->SetHolizontalDir(EHeight);

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

	const float beamWidth = 15.0f;		//�r�[���̕��B
	if (std::abs(dirW) < beamWidth and std::abs(dirH) < beamWidth and front > 0) {
		return true;
	}
	return false;
}
