#pragma once
#include "Enemy/State/IEnemyState.h"
class EnemyLongBigTargetingState;
class MuzzleFlash;
class Beam;
/// <summary>
/// ���̃N���X��EnemyLongBigTargetingState���ێ��B
/// </summary>
class EnemyLongBigAttackState final : public IEnemyState
{
public:
	~EnemyLongBigAttackState();
	void Init(IK* ik, float bulletSpeed, EnemyLongBigTargetingState* lbt);

	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;

	void SetVelocity(Vector3& v)
	{
		m_velocity = std::move(v);
	}
	void SetTarget(Vector3&& v)
	{
		m_target = v;
	}
	void SetIK(IK* ik)
	{
		m_ik = ik;
	}
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
	bool BeamJudge(IEnemy* e);
private:
	IK* m_ik = nullptr;
	float m_timer = 0.f;
	const float m_timeSpan = 0.1f;
	int m_shotCount = 0;
	const int m_maxShot = 3;
	
		Vector3			m_position = Vector3::Zero;
		float			m_damage = 0.f;
		float			m_chargeTimer = 0.f;
		float			m_endBeamTimer = 0.f;
		Beam* m_beam;

		const float		m_chargeTime = 0.f;

		bool			m_isSetPos = false;			//�r�[������������ݒ�B�Œ肾�����x�����ݒ肷��B

	Vector3 m_target = Vector3::Zero;

	Vector3 m_velocity = Vector3::Zero;
	float m_speed = 2000.f;

	EnemyLongBigTargetingState* m_lbt;

	MuzzleFlash* m_muzzleFlash;
};
