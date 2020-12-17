#pragma once
#include "Enemy/State/IEnemyState.h"
#include "Player/Player.h"

class Beam;

class Boss_FatmanBeamState final : public IEnemyState
{
public:
	Boss_FatmanBeamState();
	~Boss_FatmanBeamState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
	/// <summary>
	/// �G�t�F�N�g�̏������B
	/// </summary>
	void EffectInit();
private:
	/// <summary>
	/// ��]�̍X�V�B
	/// </summary>
	/// <param name="e">IEnemy���p�������G�̏��B</param>
	void UpdateRotation(IEnemy* e);
	/// <summary>
	/// �r�[���̓����蔻��B
	/// </summary>
	/// <param name="e">IEnemy���p�������G�̏��B</param>
	/// <returns>�r�[���ɓ������Ă��邩�ǂ����B</returns>
	bool Judge(IEnemy* e);
private:
	Vector3			m_position = Vector3::Zero;
	float			m_damage = 0.f;
	const float		m_rotTime = 2.f;
	const float		m_rotStartAngle = -90.f;
	const float		m_rotAngle = 180.f;

	Quaternion		m_startRot = Quaternion::Identity;
	Quaternion		m_rotation = Quaternion::Identity;
	float			m_countRot = 0.f;

	bool			m_isState = false;

	Beam*			m_beam = nullptr;
	Vector3			m_beamPositiion = Vector3::Zero;
	Quaternion		m_beamRotation = Quaternion::Identity;
};