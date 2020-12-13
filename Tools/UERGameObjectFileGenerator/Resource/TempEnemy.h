#pragma once

#include "Enemy/IEnemy.h"
/// <summary>
/// 
/// </summary>
class XXX final :public IEnemy
{
public:
	XXX();
	virtual ~XXX();

	void Init() override;
:block InitState
	void InitState() override;
:end
:block InitIK
	void InitIK() override;
:end
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
	//ModelRender* m_model = nullptr;
	float m_scale = 1.f;

	//Physics
	const float m_radius = 1.f;
	const float m_height = 3.f;
};