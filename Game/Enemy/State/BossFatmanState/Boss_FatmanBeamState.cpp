#include "stdafx.h"
#include "Boss_FatmanBeamState.h"
#include "Enemy/IEnemy.h"
#include "GameManager.h"

Boss_FatmanBeamState::Boss_FatmanBeamState()
{
}

Boss_FatmanBeamState::~Boss_FatmanBeamState()
{
}

void Boss_FatmanBeamState::Enter(IEnemy* e)
{
	auto& p = GameManager::GetInstance().m_player;
	m_position = p->GetPosition();

	PreRotation(e);

	//��]�p�x�̃��Z�b�g�B
	m_countRot = 0.f;
}

void Boss_FatmanBeamState::PreRotation(IEnemy* e)
{
	//����]�Əc��]���v�Z���A�v���C���[�̕����������悤�ɂ��Ă���B
	auto& p = GameManager::GetInstance().m_player;
	const auto& ppos = p->GetPosition();
	const auto& epos = e->GetPosition();
	Vector3 vecToPlayer = ppos - epos;

	//����]�B
	float angleW = atan2(vecToPlayer.x, vecToPlayer.z);
	Quaternion rot;
	rot.SetRotation(Vector3::AxisY, angleW);

	//�c��]�B
	Vector3 vecToPlayerXZ = vecToPlayer;
	vecToPlayerXZ.y = 0.0f;
	vecToPlayer.Normalize();
	vecToPlayerXZ.Normalize();
	float dot = vecToPlayer.Dot(vecToPlayerXZ);
	float angleH = acos(dot);

	//��]���B
	Vector3 axis;
	axis.Cross(vecToPlayer, vecToPlayerXZ);
	axis.Normalize();

	Quaternion rot2;
	rot2.SetRotation(axis, -angleH);

	//��]�̍����B
	rot.Multiply(rot2);

	//�����X����B
	Quaternion rot3;
	rot3.SetRotationDeg(Vector3::AxisY, -90.f);
	rot.Multiply(rot3);

	m_rotation = rot;
	
	e->GetModel()->SetRotation(rot);
}


IEnemyState* Boss_FatmanBeamState::Update(IEnemy* e)
{
	if (Judge(e)) {
		//m_damageTimer += gameTime()->GetDeltaTime();
		auto& p = GameManager::GetInstance().m_player;
		p->ApplyDamage(m_damage);
	}

	UpdateRotation(e);

	return this;
}

void Boss_FatmanBeamState::Exit(IEnemy* e)
{
}


void Boss_FatmanBeamState::UpdateRotation(IEnemy* e)
{
	const float maxTime = 5.0f;
	const float maxAngle = 180.0f;

	if (m_countRot < maxAngle) {
		//1�b�Ԃɍs����t���[�����B
		float frame = 1.f / gameTime()->GetDeltaTime();

		//1�t���[��������̉�]�ʁB
		float oneFrameAngle = maxAngle / maxTime / frame;
		
		//����]�̊p�x�ʁB
		m_countRot += oneFrameAngle;
		
		//��]�B
		Quaternion rot;
		rot.SetRotationDeg(Vector3::AxisY, m_countRot);
		rot.Multiply(m_rotation);
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

	const float beamWidth = 20.0f;		//�r�[���̕��B
	if (std::abs(dirW) < beamWidth and std::abs(dirH) < beamWidth and front > 0){
		return true;
	}
	return false;
}
