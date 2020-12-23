#pragma once
#include "Enemy/State/IEnemyState.h"
class EnemyLongBigTargetingState;
class MuzzleFlash;
class Beam;
/// <summary>
/// このクラスはEnemyLongBigTargetingStateが保持。
/// </summary>
class EnemyLongBigAttackState final : public IEnemyState
{
public:
	~EnemyLongBigAttackState();
	void Init(IK* ik, float bulletSpeed, EnemyLongBigTargetingState* lbt);

	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;

	void SetVelocity(Vector3& v)
	{
		m_velocity = std::move(v);
	}
	void SetTarget(Vector3&& v)
	{
		m_target = v;
	}
	void SetIK(IK* ik)
	{
		m_ik = ik;
	}
private:
	/// <summary>
	/// チャージする。
	/// </summary>
	/// <returns>チャージできたかどうか。</returns>
	bool Charge(IEnemy* e);
	/// <summary>
	/// ビームの当たり判定。
	/// </summary>
	/// <param name="e">IEnemyを継承した敵の情報。</param>
	/// <returns>ビームに当たっているかどうか。</returns>
	bool BeamJudge(IEnemy* e);
private:
	IK* m_ik = nullptr;
	float m_timer = 0.f;
	const float m_timeSpan = 0.1f;
	int m_shotCount = 0;
	const int m_maxShot = 3;
	
		Vector3			m_position = Vector3::Zero;
		float			m_damage = 0.f;
		float			m_chargeTimer = 0.f;
		float			m_endBeamTimer = 0.f;
		Beam* m_beam;

		const float		m_chargeTime = 0.f;

		bool			m_isSetPos = false;			//ビームを撃つ方向を設定。固定だから一度だけ設定する。

	Vector3 m_target = Vector3::Zero;

	Vector3 m_velocity = Vector3::Zero;
	float m_speed = 2000.f;

	EnemyLongBigTargetingState* m_lbt;

	MuzzleFlash* m_muzzleFlash;
};
