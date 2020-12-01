#pragma once
#include "IEnemyState.h"
#include "Player/Player.h"

class EnemyShortRangePunchState final : public IEnemyState
{
public:
	EnemyShortRangePunchState();
	~EnemyShortRangePunchState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
private:
	/// <summary>
	/// UŒ‚ŠÖ”B
	/// </summary>
	void Attack(IEnemy* e);
private:
	int m_attackIntervalCount = 30;
	//const float m_damage = Player::GetMaxHP() / 30.0f;
};
