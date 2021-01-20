#pragma once
#include "Enemy/State/IEnemyState.h"

class Boss_FatmanMainState final : public IEnemyState
{
public:
	Boss_FatmanMainState();
	~Boss_FatmanMainState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
private:
	/// <summary>
	/// 遠距離攻撃のステート遷移。
	/// </summary>
	IEnemyState* LongRangeAttack(IEnemy* e);
	/// <summary>
	/// バトルフェーズの遷移。
	/// </summary>
	void ChangeBattleState(IEnemy* e);
private:
	float m_fatTimer = 0.f;
};
