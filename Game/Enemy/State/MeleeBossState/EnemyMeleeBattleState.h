#pragma once
#include "../IEnemyState.h"

class EnemyMeleeBattleState : public IEnemyState
{
public:
	EnemyMeleeBattleState();
	virtual ~EnemyMeleeBattleState() override;
	virtual void Enter(IEnemy* e) override;
	virtual IEnemyState* Update(IEnemy* e) override;
	virtual void Exit(IEnemy* e) override;
private:
	const float m_stamina_recover_amount = 5.f;
	bool m_isPerformed = false;
	float m_tpTimer = 0.f;
	float m_thrustTimer = 0.f;

	enum class EnBattlePhase
	{
		Calm,
		Normal,
		Mad,
		NumPhase
	};

	bool m_shouldDoMadPerformance = false;

	EnBattlePhase m_battlePhase = EnBattlePhase::Calm;

};