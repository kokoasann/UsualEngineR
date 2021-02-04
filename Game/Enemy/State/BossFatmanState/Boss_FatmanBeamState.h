#pragma once
#include "Enemy/State/IEnemyState.h"
#include "Player/Player.h"

class Beam;

class Boss_FatmanBeamState final : public IEnemyState
{
public:
	Boss_FatmanBeamState();
	~Boss_FatmanBeamState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
	/// <summary>
	/// エフェクトの初期化。
	/// </summary>
	void EffectInit();
private:
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
	void Judge(IEnemy* e);
private:
	Vector3			m_position = Vector3::Zero;
	float			m_damage = 0.f;
	const float		m_rotTime = 2.f;
	const float		m_rotStartAngle = -45.0f / 2.0f;
	float			m_rotAngle = 45.0f;

	Quaternion		m_startRot = Quaternion::Identity;
	Quaternion		m_rotation = Quaternion::Identity;
	float			m_countRot = 0.f;

	bool			m_isState = false;

	std::vector< Beam*> m_beams;
	Vector3			m_beamPositiion = Vector3::Zero;
	Quaternion		m_beamRotation = Quaternion::Identity;

	float			m_maxSenkei = 0.0f;

	IK* m_ik[2];					//IK。左右のIKの情報が格納されている。
	enum IK {
		Right,
		Left,
		IKNum
	};
	CSoundSource* m_beamSE = nullptr;
};