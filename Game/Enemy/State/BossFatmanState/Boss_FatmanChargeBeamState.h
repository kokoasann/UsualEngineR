#pragma once
#include "Enemy/State/IEnemyState.h"

class Boss_FatmanChargeBeamState final : public IEnemyState
{
public:
	Boss_FatmanChargeBeamState();
	~Boss_FatmanChargeBeamState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
private:
	/// <summary>
	/// チャージする。
	/// </summary>
	/// <returns>チャージできたかどうか。</returns>
	bool Charge();
	/// <summary>
	/// ビームの当たり判定。
	/// </summary>
	/// <param name="e">IEnemyを継承した敵の情報。</param>
	/// <returns>ビームに当たっているかどうか。</returns>
	bool Beam(IEnemy* e);
private:
	Vector3			m_position = Vector3::Zero;
	float			m_damage = 0.f;
	float			m_chargeTimer = 0.f;
	float			m_endBeamTimer = 0.f;
};
