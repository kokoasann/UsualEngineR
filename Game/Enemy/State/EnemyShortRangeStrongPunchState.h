#pragma once
#include "IEnemyState.h"
#include "Player/Player.h"

class EnemyShortRangeStrongPunchState final : public IEnemyState
{
public:
	EnemyShortRangeStrongPunchState();
	~EnemyShortRangeStrongPunchState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
private:
	const float		m_damage = Player::GetMaxHP() / 15.0f;
};
