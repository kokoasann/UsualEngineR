#pragma once
#include "Enemy/State/IEnemyState.h"
class Beam;

class Boss_FatmanChargeBeamAndShootingState final : public IEnemyState
{
public:
	Boss_FatmanChargeBeamAndShootingState();
	~Boss_FatmanChargeBeamAndShootingState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
	void  OnAttacked(IEnemy* e);
private:
	//���߃r�[���B
	/// <summary>
	/// ���߃r�[���̏����������B
	/// </summary>
	void InitChargeBeam(IEnemy* e);
	/// <summary>
	/// ���߃r�[���̏����B
	/// </summary>
	void ChargeBeam(IEnemy* e);
	/// <summary>
	/// �`���[�W����B
	/// </summary>
	/// <returns>�`���[�W�ł������ǂ����B</returns>
	bool Charge(IEnemy* e);
	/// <summary>
	/// �r�[���̓����蔻��B
	/// </summary>
	/// <param name="e">IEnemy���p�������G�̏��B</param>
	/// <returns>�r�[���ɓ������Ă��邩�ǂ����B</returns>
	bool BeamJudge(IEnemy* e, int ikNo);

	//���ˁB
	/// <summary>
	/// ���˂̏����������B
	/// </summary>
	void InitShooting(IEnemy* e);
	/// <summary>
	/// ���˂̏����B
	/// </summary>
	void Shooting(IEnemy* e);
	/// <summary>
	/// �e�𐶐�����֐��B
	/// </summary>
	void BulletGenerate(IEnemy* e);
private:
	const int				IK_NUM = 2;					//IK�̐��B
	Quaternion				m_startRot = Quaternion::Identity;	//���f�����ŏ��Ɍ����Ă���p�x�B
	float					m_sumLerp = 0.0f;
	//���߃r�[���B
	Vector3					m_position = Vector3::Zero;
	float					m_chargeDamage = 0.f;
	float					m_chargeTimer = 0.f;
	float					m_endBeamTimer = 0.f;
	std::vector<Beam*>		m_beams;
	const float				m_chargeTime = 4.0f;
	bool					m_isSetPos = false;			//�r�[������������ݒ�B�Œ肾�����x�����ݒ肷��B
	IK*						m_chargebeamIk[2];			//IK�B���E��IK�̏�񂪊i�[����Ă���B
	bool					m_isEndChargeBeam = false;	//���߃r�[�����I��������ǂ����B
	float					m_sumMoveSpeed = 0.0f;
	bool					m_isKnockBackCB = false;
	bool					m_isChargeSound = false;
	CSoundSource*			m_chargeSE = nullptr;
	CSoundSource*			m_beamSE = nullptr;

	//���ˁB
	float					m_shotTimer = 0.f;
	float					m_stateTimer = 0.f;
	float					m_shootingDamage = 0.f;
	IK*						m_shootingIk[2];			//IK�B���E��IK�̏�񂪊i�[����Ă���B
	bool					m_isEndShooting = false;	//���˂��I��������ǂ����B
	bool					m_isKnockBackSH = false;
	CSoundSource*			m_shootSE = nullptr;
};
