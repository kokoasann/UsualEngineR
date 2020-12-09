#pragma once
#include "Enemy/State/IEnemyState.h"
#include "Player/Player.h"

class Boss_FatmanBeamState final : public IEnemyState
{
public:
	Boss_FatmanBeamState();
	~Boss_FatmanBeamState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
private:
	/// <summary>
	/// ビームを撃つ前の初期位置を設定する関数。
	/// </summary>
	/// <param name="e">IEnemyを継承した敵の情報。</param>
	void PreRotation(IEnemy* e);
	/// <summary>
	/// 回転の更新。
	/// </summary>
	/// <param name="e">IEnemyを継承した敵の情報。</param>
	void UpdateRotation(IEnemy* e);
	/// <summary>
	/// ビームの当たり判定。
	/// </summary>
	/// <param name="e">IEnemyを継承した敵の情報。</param>
	/// <returns>ビームに当たっているかどうか。</returns>
	bool Judge(IEnemy* e);
private:
	Vector3			m_position = Vector3::Zero;
	const float		m_damage = Player::GetMaxHP() / 2000.f;
	float			m_damageTimer = 0.f;

	Quaternion		m_rotation = Quaternion::Identity;
	float			m_countRot = 0.f;
	float			m_timer= 0.f;
};