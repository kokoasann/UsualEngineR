#pragma once
#include "IEnemyState.h"
#include "Enemy/Zako/Zako_ShortRangeMonster.h"

class EnemyShortRangeComingState final : public IEnemyState
{
public:
	EnemyShortRangeComingState();
	~EnemyShortRangeComingState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
private:
	/// <summary>
	/// ˆÚ“®B
	/// </summary>
	void Move(IEnemy* e);
private:
};
