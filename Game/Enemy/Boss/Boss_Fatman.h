#pragma once
#include "GameManager.h"
#include "Enemy/IEnemy.h"

/// <summary>
/// ファットマン。
/// </summary>
class Boss_Fatman final :public IEnemy
{
public:
	Boss_Fatman();
	virtual ~Boss_Fatman();

	void Init() override;
	void InitState() override;
	void Execute() override;
	void Terminate() override;
	Player::EnAttackType GetDropID() override
	{
		return Player::EnAttackType::enNone;
	}

	
	enum class EnStateEX :int
	{
		enAttackC = TO_INT(IEnemy::EnState::enNumState),
		enAttackD,
		enAttackE,
		enTakeDistance,
		Num,
	};

	enum class EnAnimEX :int
	{
		Expand = TO_INT(IEnemy::EnAnimation::enNumAnim),
		Num,
	};

	/// <summary>
	/// ビームを撃つ前の初期位置を設定する関数。
	/// </summary>
	/// <param name="e">IEnemyを継承した敵の情報。</param>
	/// <returns>プレイヤーの方向に向くQuaternion。</returns>
	static Quaternion EnemyToPlayerRotation(IEnemy* e)
	{
		//横回転と縦回転を計算し、プレイヤーの方向を向くようにしている。
		auto& p = GameManager::GetInstance().m_player;
		const auto& ppos = p->GetPosition();
		const auto& epos = e->GetPosition();
		Vector3 vecToPlayer = ppos - epos;

		//横回転。
		float angleW = atan2(vecToPlayer.x, vecToPlayer.z);
		Quaternion rot;
		rot.SetRotation(Vector3::AxisY, angleW);

		//縦回転。
		Vector3 vecToPlayerXZ = vecToPlayer;
		vecToPlayerXZ.y = 0.0f;
		vecToPlayer.Normalize();
		vecToPlayerXZ.Normalize();
		float dot = vecToPlayer.Dot(vecToPlayerXZ);
		float angleH = acos(dot);

		//回転軸。
		Vector3 axis;
		axis.Cross(vecToPlayer, vecToPlayerXZ);
		axis.Normalize();

		Quaternion rot2;
		rot2.SetRotation(axis, -angleH);

		//回転の合成。
		rot.Multiply(rot2);

		return rot;
	}

	/// <summary>
	/// ダメージを計算。
	/// 秒数に応じて1フレームに与えるべきダメージ量を計算してくれる。
	/// </summary>
	/// <param name="time">秒数。</param>
	/// <returns>ダメージ量。</returns>
	static float CalcDamage(float time) {
		auto& p = GameManager::GetInstance().m_player;
		float hp = p->GetMaxHP();
		//1秒間に行われるフレーム数。
		float frame = 1.f / gameTime()->GetDeltaTime();
		//1フレームあたりのダメージ量。
		float damage;
		damage = hp / time / frame;
		return damage;
	}
private:
	float m_scale = 1.f;

	//Physics
	const float m_radius = 2.f;
	const float m_height = 3.f;
};
