#pragma once

#include "Enemy/IEnemy.h"
#include "Enemy/State/EnemyDeadState.h"
#include "Enemy/State/EnemyIdleState.h"
#include "Enemy/State/BossFatmanState/Boss_FatmanMainState.h"
#include "Enemy/State/BossFatmanState/Boss_FatmanBeamState.h"
/// <summary>
/// ファットマン。
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
		Expand = TO_INT(IEnemy::EnState::enNumState),
		Num,
	};

	enum class EnAnimEX :int
	{
		Expand = TO_INT(IEnemy::EnAnimation::enNumAnim),
		Num,
	};

private:
	float m_scale = 1.f;

	//Physics
	const float m_radius = 1.f;
	const float m_height = 3.f;
};
