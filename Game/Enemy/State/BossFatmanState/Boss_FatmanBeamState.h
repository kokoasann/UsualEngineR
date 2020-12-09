#pragma once
#include "Enemy/State/IEnemyState.h"
#include "Player/Player.h"

class Boss_FatmanBeamState final : public IEnemyState
{
public:
	Boss_FatmanBeamState();
	~Boss_FatmanBeamState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
private:
	/// <summary>
	/// �r�[�������O�̏����ʒu��ݒ肷��֐��B
	/// </summary>
	/// <param name="e">IEnemy���p�������G�̏��B</param>
	void PreRotation(IEnemy* e);
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
	const float		m_damage = Player::GetMaxHP() / 2000.f;
	float			m_damageTimer = 0.f;

	Quaternion		m_rotation = Quaternion::Identity;
	float			m_countRot = 0.f;
	float			m_timer= 0.f;
};