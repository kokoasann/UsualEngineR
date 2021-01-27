#pragma once
#include "Enemy/State/IEnemyState.h"

class Boss_FatmanAngryPerform;


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

	//演出関係。
	bool m_isPerformed = false;
	bool m_isAngryPerformed = false;
	bool m_isBmp = false;
	Boss_FatmanAngryPerform* m_bap = nullptr;

	//IK関係。
	const int IK_NUM = 2;
	IK* m_BeamIk[2];					//IK。左右のIKの情報が格納されている。
	IK* m_ShootIk[2];					//IK。左右のIKの情報が格納されている。
};
