#pragma once
#include "Enemy/State/IEnemyState.h"
class Beam;

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
	bool Charge(IEnemy* e);
	/// <summary>
	/// ビームの当たり判定。
	/// </summary>
	/// <param name="e">IEnemyを継承した敵の情報。</param>
	/// <returns>ビームに当たっているかどうか。</returns>
	bool BeamJudge(IEnemy* e, int boneNo);
private:
	Vector3			m_position = Vector3::Zero;
	float			m_damage = 0.f;
	float			m_chargeTimer = 0.f;
	float			m_endBeamTimer = 0.f;
	std::vector< Beam*> m_beams;

	const float		m_chargeTime = 4.0f;

	bool			m_isSetPos = false;			//ビームを撃つ方向を設定。固定だから一度だけ設定する。
	Bone*			m_bone[2];					//骨のID。左右の骨の情報が格納されている。
	enum Bone{
		Right,
		Left,
		BoneNum
	};
};
