#pragma once
#include "IEnemyState.h"

class EnemyExplosionBombState final : public IEnemyState
{
public:
	EnemyExplosionBombState();
	~EnemyExplosionBombState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
private:
};
