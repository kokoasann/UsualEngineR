#include "stdafx.h"
#include "Boss_FatmanChargeBeamState.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/Boss/Boss_Fatman.h"

Boss_FatmanChargeBeamState::Boss_FatmanChargeBeamState()
{
}

Boss_FatmanChargeBeamState::~Boss_FatmanChargeBeamState()
{
}

void Boss_FatmanChargeBeamState::Enter(IEnemy* e)
{
	auto& p = GameManager::GetInstance().m_player;
	m_position = p->GetPosition();

	e->GetModel()->SetRotation(Boss_Fatman::EnemyToPlayerRotation(e));

	//�_���[�W�����v�Z�B
	const float time = 10.f;
	m_damage = Boss_Fatman::CalcDamage(time);

	//�^�C�}�[�̃��Z�b�g�B
	m_chargeTimer = 0.f;
	m_endBeamTimer = 0.f;
}

IEnemyState* Boss_FatmanChargeBeamState::Update(IEnemy* e)
{
	if (Charge()) {
		m_endBeamTimer += gameTime()->GetDeltaTime();
		const float endTime = 2.f;
		if (m_endBeamTimer < endTime) {
			if (Beam(e)) {
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
		else {
			return e->GetState(TO_INT(IEnemy::EnState::enBattleState));
		}
	}
	return this;
}

void Boss_FatmanChargeBeamState::Exit(IEnemy* e)
{
}

bool Boss_FatmanChargeBeamState::Charge()
{
	const float chargeTime = 3.f;
	m_chargeTimer += gameTime()->GetDeltaTime();
	if (m_chargeTimer > chargeTime) {
		return true;
	}
	return false;
}

bool Boss_FatmanChargeBeamState::Beam(IEnemy* e)
{
	//�r�[���̕��̔���B
	auto& epos = e->GetPosition();

	//�G����v���C���[�Ɍ������x�N�g���B
	//m_position�͍ŏ��Ƀ��b�N�I�������Ƃ��̃v���C���[�̈ʒu�B
	Vector3 vecEtoP = m_position - epos;

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

	const float beamWidth = 15.0f;		//�r�[���̕��B
	if (std::abs(dirW) < beamWidth and std::abs(dirH) < beamWidth and front > 0) {
		return true;
	}
	return false;
}
