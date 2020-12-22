#include "stdafx.h"
#include "Boss_FatmanChargeBeamState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Boss/Boss_Fatman.h"
#include "Effect/Beam.h"

Boss_FatmanChargeBeamState::Boss_FatmanChargeBeamState()
{
	//���̐������r�[���𐶐�����B
	for (int i = 0; i < BoneNum; i++) {
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

	//�����B
	auto ske = e->GetModel()->GetModel().GetSkelton();
	m_bone[Right] = ske->GetBone(ske->FindBoneID(L"Beam_IK_R"));
	m_bone[Left] = ske->GetBone(ske->FindBoneID(L"Beam_IK_L"));
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
			}
			//�r�[����2�{����̂�2�񔻒���s���B
			for (int i = 0; i < BoneNum; i++) {
				//�U������B
				if (BeamJudge(e,i)) {
					//�v���C���[�����ł����猂�����Ƃ��B
					const float flyRange = 5.f;
					auto epos = e->GetPosition();
					auto& p = GameManager::GetInstance().m_player;
					auto ppos = p->GetPosition();
					if (std::abs(ppos.y - epos.y) > flyRange) {
						p->ApplyDamage(m_damage, true, Vector3::Zero);
					}
					else {
						p->ApplyDamage(m_damage);
					}
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

	for (int i = 0; i < BoneNum; i++) {
		//�G�t�F�N�g�̐ݒ�B
		//�G�t�F�N�g�𗭂ߏ�Ԃɐݒ�B
		m_beams[i]->SetChange(true);

		//�傫���������߂ɐݒ�B
		m_beams[i]->SetSca(Vector3::One * 0.03);

		//���̍��W�̎擾�B
		const auto epos = m_bone[i]->GetWorldMatrix().GetTransrate();
		m_beams[i]->SetPos(epos);

		//���f������v���C���[�܂ł̕�����ݒ�B
		auto& p = GameManager::GetInstance().m_player;
		const auto& ppos = p->GetPosition();
		Vector3 vecEtoP = ppos - epos;
		m_beams[i]->SetToPlayerDir(vecEtoP);

		//�O�ρB���s�����c�̃x�N�g���B
		Vector3 EHeight;
		EHeight.Cross(vecEtoP, Vector3::Right);
		EHeight.Normalize();
		m_beams[i]->SetHolizontalDir(EHeight);

		//�G�t�F�N�g�̍Đ��B
		m_beams[i]->Play();
	}
	
	//���f�����v���C���[�̕����������悤�ɐݒ�B
	e->GetModel()->SetRotation(Boss_Fatman::EnemyToPlayerRotation(e));	
	return false;
}

bool Boss_FatmanChargeBeamState::BeamJudge(IEnemy* e, int boneNo)
{
	//�G�t�F�N�g�̗��ߏ�Ԃ������B
	m_beams[boneNo]->SetChange(false);
	//�傫����傫�߂ɐݒ�B
	m_beams[boneNo]->SetSca(Vector3::One * 0.3);

	//�r�[���̕��̔���B
	//�G����v���C���[�Ɍ������x�N�g���B
	//m_position�͍ŏ��Ƀ��b�N�I�������Ƃ��̃v���C���[�̈ʒu�B
	auto epos = m_bone[boneNo]->GetWorldMatrix().GetTransrate();

	Vector3 vecEtoP = m_position - epos;
	m_beams[boneNo]->SetToPlayerDir(vecEtoP);

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

	m_beams[boneNo]->Play();
	m_beams[boneNo]->SetPos(epos);

	const float beamWidth = 15.0f;		//�r�[���̕��B
	if (std::abs(dirW) < beamWidth and std::abs(dirH) < beamWidth and front > 0) {
		return true;
	}
	return false;
}
