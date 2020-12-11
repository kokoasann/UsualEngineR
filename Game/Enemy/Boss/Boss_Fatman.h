#pragma once
#include "GameManager.h"
#include "Enemy/IEnemy.h"

/// <summary>
/// �t�@�b�g�}���B
/// </summary>
class Boss_Fatman final :public IEnemy
{
public:
	Boss_Fatman();
	virtual ~Boss_Fatman();

	void Init() override;
	void InitState() override;
	void Execute() override;
	void Terminate() override;
	Player::EnAttackType GetDropID() override
	{
		return Player::EnAttackType::enNone;
	}

	
	enum class EnStateEX :int
	{
		enAttackC = TO_INT(IEnemy::EnState::enNumState),
		enAttackD,
		enAttackE,
		enTakeDistance,
		Num,
	};

	enum class EnAnimEX :int
	{
		Expand = TO_INT(IEnemy::EnAnimation::enNumAnim),
		Num,
	};

	/// <summary>
	/// �r�[�������O�̏����ʒu��ݒ肷��֐��B
	/// </summary>
	/// <param name="e">IEnemy���p�������G�̏��B</param>
	/// <returns>�v���C���[�̕����Ɍ���Quaternion�B</returns>
	static Quaternion EnemyToPlayerRotation(IEnemy* e)
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

		return rot;
	}

	/// <summary>
	/// �_���[�W���v�Z�B
	/// �b���ɉ�����1�t���[���ɗ^����ׂ��_���[�W�ʂ��v�Z���Ă����B
	/// </summary>
	/// <param name="time">�b���B</param>
	/// <returns>�_���[�W�ʁB</returns>
	static float CalcDamage(float time) {
		auto& p = GameManager::GetInstance().m_player;
		float hp = p->GetMaxHP();
		//1�b�Ԃɍs����t���[�����B
		float frame = 1.f / gameTime()->GetDeltaTime();
		//1�t���[��������̃_���[�W�ʁB
		float damage;
		damage = hp / time / frame;
		return damage;
	}
private:
	float m_scale = 1.f;

	//Physics
	const float m_radius = 2.f;
	const float m_height = 3.f;
};
