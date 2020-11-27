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
	void Execute() override;
	void Terminate() override;
	Player::EnAttackType GetDropID() override
	{
		return Player::EnAttackType::enNone;
	}

	
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
	//Model
	ModelRender* m_model = nullptr;
	float m_scale = 1.f;

	//Physics
	const float m_radius = 1.f;
	const float m_height = 3.f;
};