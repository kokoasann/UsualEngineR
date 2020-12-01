#pragma once

#include "Enemy/IEnemy.h"
#include "Enemy/State/EnemyDeadState.h"
#include "Enemy/State/EnemyIdleState.h"
#include "Enemy/State/EnemyShortRangeComingState.h"
#include "Enemy/State/EnemyShortRangePunchState.h"

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
		enComing = TO_INT(IEnemy::EnState::enBattleState),
		Num,
	};

	enum class EnAnimEX :int
	{
		Expand = TO_INT(IEnemy::EnAnimation::enNumAnim),
		Num,
	};

	/*enum class EnState {
		enComing = TO_INT(IEnemy::EnState::enNumState),
		enNumState
	};*/
private:
	//Model
	ModelRender* m_model = nullptr;

	float m_scale = 1.f;

	//Physics
	const float m_radius = 1.f;
	const float m_height = 3.f;
};
