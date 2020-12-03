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
	const float		m_damage = Player::GetMaxHP() / 30.0f;
};
