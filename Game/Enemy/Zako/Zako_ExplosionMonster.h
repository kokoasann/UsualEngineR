#pragma once

#include "Enemy/IEnemy.h"
#include "Enemy/State/EnemyDeadState.h"
#include "Enemy/State/EnemyIdleState.h"
#include "Enemy/State/EnemyExplosionKamikazeState.h"
#include "Enemy/State/EnemyExplosionBombState.h"

/// <summary>
/// 
/// </summary>
class Zako_ExplosionMonster final :public IEnemy
{
public:
	Zako_ExplosionMonster();
	virtual ~Zako_ExplosionMonster();

	void Init() override;
	void InitState() override;
	void Execute() override;
	void Terminate() override;
	Player::EnAttackType GetDropID() override
	{
		return Player::EnAttackType::enNone;
	}

	////�f�t�H���g�̃A�j���[�V�����X�e�[�g�p�B
	//void PlayAnimation(const EnAnimation& anim, const float interpolate = 0.2f) override {
	//	m_model->Play(static_cast<int>(anim), interpolate);
	//}

	////�g�����ꂽ�A�j���[�V�����X�e�[�g�p�B
	//void PlayAnimation(const int animNo, const float interpolate = 0.2f) override {
	//	m_model->Play(animNo, interpolate);
	//}
	
	enum class EnStateEX :int
	{
		Expand = TO_INT(IEnemy::EnState::enNumState),
		Num,
	};

	enum class EnAnimEX :int
	{
		Expand = TO_INT(IEnemy::EnAnimation::enNumAnim),
		Num,
	};
private:
	/// <summary>
	/// �A�j���[�V�����̏����ݒ�B
	/// </summary>
	void InitAnimation();
private:
	float m_scale = 1.f;

	//Physics
	const float m_radius = 1.f;
	const float m_height = 3.f;
};
