#pragma once
#include "Enemy/State/IEnemyState.h"

class EnemyShortBigIdle final : public IEnemyState
{
public:
	EnemyShortBigIdle();
	~EnemyShortBigIdle();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
	void OnAttacked(IEnemy* e) override
	{
		m_isAttacked = true;
	}
private:
	bool m_isAttacked = false;
};
