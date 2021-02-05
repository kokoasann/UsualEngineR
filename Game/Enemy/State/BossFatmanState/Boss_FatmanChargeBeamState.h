#pragma once
#include "Enemy/State/IEnemyState.h"
class Beam;

class Boss_FatmanChargeBeamState final : public IEnemyState
{
public:
	Boss_FatmanChargeBeamState();
	~Boss_FatmanChargeBeamState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
private:
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
private:
	Vector3			m_position = Vector3::Zero;
	float			m_damage = 0.f;
	float			m_chargeTimer = 0.f;
	float			m_endBeamTimer = 0.f;
	std::vector< Beam*> m_beams;

	const float		m_chargeTime = 4.0f;

	bool			m_isSetPos = false;			//�r�[������������ݒ�B�Œ肾�����x�����ݒ肷��B
	bool			m_isKnockBack = false;			
	IK*				m_ik[2];					//IK�B���E��IK�̏�񂪊i�[����Ă���B
	enum IK{
		Right,
		Left,
		IKNum
	};
	CSoundSource* m_beamSE = nullptr;
	bool m_isBeamSound = false;
};
