#include "stdafx.h"
#include "Boss_FatmanBeamState.h"
#include "Enemy/IEnemy.h"
#include "GameManager.h"
#include "Enemy/Boss/Boss_Fatman.h"
#include "Effect/Beam.h"

Boss_FatmanBeamState::Boss_FatmanBeamState()
{
	BeamEffectInitData bid;
	//���̐������r�[���𐶐�����B
	for (int i = 0; i < IKNum; i++) {
		Beam* beam = NewGO<Beam>(0);
		beam->Init(bid);
		beam->SetSca(Vector3::One * 0.03);
		m_beams.push_back(beam);
	}
}

Boss_FatmanBeamState::~Boss_FatmanBeamState()
{
	for (int i = 0; i < m_beams.size(), i++;) {
		DeleteGO(m_beams.at(i));
	}
}

//���f���ɉ�]�̔��f�͂����Ȃ���������Ȃ��B
void Boss_FatmanBeamState::Enter(IEnemy* e)
{
	auto& p = GameManager::GetInstance().m_player;
	m_position = p->GetPosition();
	m_position.y += 6.0f;

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

	m_isState = false;

	//IK���B
	for (int i = 0; i < IKNum; i++) {
		m_ik[i] = e->GetIK(TO_INT(IEnemy::EnIK::enArm_L) + i);
	}

	m_maxSenkei = 0;
}

IEnemyState* Boss_FatmanBeamState::Update(IEnemy* e)
{
	UpdateRotation(e);
	
	if (m_isState && m_maxSenkei > 1.0f) {
		return e->GetState(TO_INT(IEnemy::EnState::enBattleState));
	}

	if (m_countRot > 0.f) {
		Judge(e);
	}

	//�v���C���[�̉����������߂�B
	const auto epos = e->GetPosition();
	Vector3 vec = m_position - epos;
	vec.Normalize();
	Vector3 XZ;
	XZ.Cross(vec, Vector3::Up);

	//���`�⊮���ăv���C���[�̍�����E�֓������B
	Vector3 senkeihokan;
	const float moveRange = 1.0f;
	float oneFrameMove = moveRange / m_rotTime * gameTime()->GetDeltaTime();
	m_maxSenkei += oneFrameMove;
	if (m_maxSenkei <= 1.0f) {
		const float range = 37.0f;
		senkeihokan.Lerp(m_maxSenkei, XZ * range + m_position, XZ * -range + m_position);
	}

	for (int i = 0; i < IKNum; i++) {
		m_ik[i]->SetNextTarget(senkeihokan);
		m_beams[i]->Play();
		m_beams[i]->SetPos(m_ik[i]->GetEffectorBone()->GetWorldMatrix().GetTransrate());
		m_beams[i]->SetRot(m_ik[i]->GetEffectorBone()->GetWorldMatrix().GetRotate());
	}

	return this;
}

void Boss_FatmanBeamState::Exit(IEnemy* e)
{
}

void Boss_FatmanBeamState::EffectInit()
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
	else {
		m_isState = true;
	}
}

//�r�[���̕��̔���B
void Boss_FatmanBeamState::Judge(IEnemy* e)
{
	for (int i = 0; i < IKNum; i++) {
		//�r�[���𔭎˂��Ă���������v�Z�B
		//�{�[���̑O�����ɉ�]�ʂ����蓖�āA�������v�Z�B
		Vector3 vecBeamDir = Vector3::AxisY;
		m_ik[i]->GetEffectorBone()->GetWorldMatrix().GetRotate().Apply(vecBeamDir);

		//�O�ρB�������ɐL�т��AvecBeamDir�ɒ��s����x�N�g���B
		Vector3 EWidth;
		EWidth.Cross(vecBeamDir, Vector3::Up);
		EWidth.Normalize();

		//ppos�͌��݂̃v���C���[�̈ʒu�B
		const auto& p = GameManager::GetInstance().m_player;
		const auto& ppos = p->GetPosition();
		Vector3 bonePos = m_ik[i]->GetEffectorBone()->GetWorldMatrix().GetTransrate();
		Vector3 vecArmtoCurrentP = ppos - bonePos;

		//�v���C���[�ƓG����ׂ��Ɖ��肵���Ƃ��̋����B
		float distance = EWidth.Dot(vecArmtoCurrentP);

		//���ʂɂ��邩�ǂ������肷�邽�߂̓��ρB
		//�}�C�i�X����������B
		float front = vecBeamDir.Dot(vecArmtoCurrentP);

		const float beamWidth = 15.0f;		//�r�[���̕��B
		if (std::abs(distance) < beamWidth and front > 0) {
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
