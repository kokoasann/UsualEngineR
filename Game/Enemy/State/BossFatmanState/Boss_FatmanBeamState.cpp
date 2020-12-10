#include "stdafx.h"
#include "Boss_FatmanBeamState.h"
#include "Enemy/IEnemy.h"
#include "GameManager.h"
#include "Enemy/Boss/Boss_Fatman.h"

Boss_FatmanBeamState::Boss_FatmanBeamState()
{
}

Boss_FatmanBeamState::~Boss_FatmanBeamState()
{
}

//���f���ɉ�]�̔��f�͂����Ȃ���������Ȃ��B
void Boss_FatmanBeamState::Enter(IEnemy* e)
{
	auto& p = GameManager::GetInstance().m_player;
	m_position = p->GetPosition();

	//�����X����B
	Quaternion rot;
	rot.SetRotationDeg(Vector3::AxisY, m_rotStartAngle);
	rot.Multiply(Boss_Fatman::EnemyToPlayerRotation(e));
	m_startRot = rot;
	e->GetModel()->SetRotation(rot);

	//��]�p�x�̃��Z�b�g�B
	m_countRot = 0.f;

	//�_���[�W�����v�Z�B
	const float time = 20.f;
	m_damage = Boss_Fatman::CalcDamage(time);
}

IEnemyState* Boss_FatmanBeamState::Update(IEnemy* e)
{
	UpdateRotation(e);
	
	if (m_countRot > 0.f) {
		if (Judge(e)) {
			auto& p = GameManager::GetInstance().m_player;
			p->ApplyDamage(m_damage);
		}
	}

	return this;
}

void Boss_FatmanBeamState::Exit(IEnemy* e)
{
}

void Boss_FatmanBeamState::UpdateRotation(IEnemy* e)
{
	if (m_countRot < m_rotAngle) {
		//1�b�Ԃɍs����t���[�����B
		float frame = 1.f / gameTime()->GetDeltaTime();

		//1�t���[��������̉�]�ʁB
		float oneFrameAngle = m_rotAngle / m_rotTime / frame;
		
		//�p�x�̍��v�ʁB
		m_countRot += oneFrameAngle;
		
		//��]�B
		m_rotation.SetRotationDeg(Vector3::AxisY, m_countRot);
		Quaternion rot = m_rotation;
		rot.Multiply(m_startRot);
		e->GetModel()->SetRotation(rot);
	}
}

bool Boss_FatmanBeamState::Judge(IEnemy* e)
{
	//�r�[���̕��̔���B
	auto& epos = e->GetPosition();

	//�G����v���C���[�Ɍ������x�N�g���B
	//m_position�͍ŏ��Ƀ��b�N�I�������Ƃ��̃v���C���[�̈ʒu�B
	Vector3 vecEtoP = m_position - epos;
	m_rotation.Apply(vecEtoP);

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
	if (std::abs(dirW) < beamWidth and std::abs(dirH) < beamWidth and front > 0){
		return true;
	}
	return false;
}
