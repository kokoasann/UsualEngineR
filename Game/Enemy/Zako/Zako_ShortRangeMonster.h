#pragma once

#include "Enemy/IEnemy.h"


/// <summary>
/// 
/// </summary>
class Zako_ShortRangeMonster final :public IEnemy
{
public:
	Zako_ShortRangeMonster();
	virtual ~Zako_ShortRangeMonster();

	void Init() override;
	void InitState() override;
	void InitIK() override;
	void Execute() override;
	void Terminate() override;
	Player::EnAttackType GetDropID() override
	{
		return Player::EnAttackType::enNone;
	}
	
	//デフォルトのアニメーションステート用。
	void PlayAnimation(const EnAnimation& anim, const float interpolate = 0.2f) override {
		m_model->Play(static_cast<int>(anim), interpolate);
	}

	//拡張されたアニメーションステート用。
	void PlayAnimation(const int animNo, const float interpolate = 0.2f) override {
		m_model->Play(animNo, interpolate);
	}

	enum class EnStateEX :int
	{
		enComing = TO_INT(IEnemy::EnState::enBattleState),
		enDance = TO_INT(IEnemy::EnState::enNumState),
		enLongJampAttack,
		Num,
	};

	enum class EnAnimEX :int
	{
		enDance = TO_INT(IEnemy::EnAnimation::enNumAnim),
		Num,
	};
private:
	/// <summary>
	/// アニメーションの初期設定。
	/// </summary>
	void InitAnimation();
private:
	float m_scale = 1.5f;

	//Physics
	const float m_radius = 2.f;
	const float m_height = 3.f;
};
