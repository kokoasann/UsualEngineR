#pragma once
#include "GameManager.h"
#include "Enemy/IEnemy.h"
#include "Effect/ExplosionEffect.h"

enum class EnBattlePhase
{
	Normal,
	Mad,
	Tired,
	NumPhase
};

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
	void InitIK() override;
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
		enAngry = TO_INT(IEnemy::EnAnimation::enNumAnim),
		enbackStep,
		Num,
	};

	/// <summary>
	/// �r�[�������O�̏����ʒu��ݒ肷��֐��B
	/// </summary>
	/// <param name="e">IEnemy���p�������G�̏��B</param>
	/// <returns>�v���C���[�̕����Ɍ���Quaternion�B</returns>
	static const Quaternion EnemyToPlayerRotation(IEnemy* e, bool isRotY = true)
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

		if (isRotY) {
			//�c��]�B
			Vector3 vecToPlayerXZ = vecToPlayer;
			vecToPlayerXZ.y = 0.0f;
			vecToPlayer.Normalize();
			vecToPlayerXZ.Normalize();
			float dot = vecToPlayer.Dot(vecToPlayerXZ);
			//�͈͂𐧌�����B
			dot = min(dot, -1);
			dot = max(dot, 1);
			float angleH = acos(dot);

			//��]���B
			Vector3 axis;
			axis.Cross(vecToPlayer, vecToPlayerXZ);
			axis.Normalize();

			Quaternion rot2;
			rot2.SetRotation(axis, -angleH);

			//��]�̍����B
			rot.Multiply(rot2);
		}

		return rot;
	}

	/// <summary>
	/// �_���[�W���v�Z�B
	/// �b���ɉ�����1�t���[���ɗ^����ׂ��_���[�W�ʂ��v�Z���Ă����B
	/// </summary>
	/// <param name="time">�b���B</param>
	/// <returns>�_���[�W�ʁB</returns>
	static const float CalcDamage(float time) {
		auto& p = GameManager::GetInstance().m_player;
		float hp = p->GetMaxHP();
		//1�b�Ԃɍs����t���[�����B
		float frame = 1.f / gameTime()->GetDeltaTime();
		//1�t���[��������̃_���[�W�ʁB
		float damage;
		damage = hp / time / frame;
		return damage;
	}

	//�f�t�H���g�̃A�j���[�V�����X�e�[�g�p�B
	void PlayAnimation(const EnAnimation& anim, const float interpolate = 0.2f) override {
		m_model->Play(static_cast<int>(anim), interpolate);
	}

	//�g�����ꂽ�A�j���[�V�����X�e�[�g�p�B
	void PlayAnimation(const int animNo, const float interpolate = 0.2f) override {
		m_model->Play(animNo, interpolate);
	}

	static void SetBattlePhase(EnBattlePhase en) {
		m_battlePhase = en;
	}

	static EnBattlePhase GetCurrentBattlePhase() {
		return m_battlePhase;
	}


	void Explode() override {
		auto forward = GetForward();
		forward *= -30.f;
		m_explodeEffect->SetPos(m_position + forward);
		//m_explodeEffect->SetPos(Vector3::One);
		m_explodeEffect->Play();
	}
	static const float TAKE_DISTANCE;

private:
	/// <summary>
	/// �A�j���[�V�����̏������B
	/// </summary>
	void InitAnimation();
private:
	float m_scale = 1.f;

	//Physics
	const float m_radius = 6.f;
	const float m_height = 9.f;

	//�o�g���t�F�[�Y�B
	static EnBattlePhase m_battlePhase;

	//�T�E���h�B
	/*CSoundSource*	m_bgm = nullptr;
	float			m_volume = 0.0f;*/
};
