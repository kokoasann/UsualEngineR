#pragma once
#include "IEnemyState.h"
#include "Player/Player.h"

class EnemyExplosionBombState final : public IEnemyState
{
public:
	EnemyExplosionBombState();
	~EnemyExplosionBombState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
private:
	const float		m_damage = Player::GetMaxHP() / 20.0f;
};
