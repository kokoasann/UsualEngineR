#pragma once
#include "Enemy/State/IEnemyState.h"
class Beam;

class Boss_FatmanChargeBeamAndShootingState final : public IEnemyState
{
public:
	Boss_FatmanChargeBeamAndShootingState();
	~Boss_FatmanChargeBeamAndShootingState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
	void  OnAttacked(IEnemy* e);
private:
	//溜めビーム。
	/// <summary>
	/// 溜めビームの初期化処理。
	/// </summary>
	void InitChargeBeam(IEnemy* e);
	/// <summary>
	/// 溜めビームの処理。
	/// </summary>
	void ChargeBeam(IEnemy* e);
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
	bool BeamJudge(IEnemy* e, int ikNo);

	//乱射。
	/// <summary>
	/// 乱射の初期化処理。
	/// </summary>
	void InitShooting(IEnemy* e);
	/// <summary>
	/// 乱射の処理。
	/// </summary>
	void Shooting(IEnemy* e);
	/// <summary>
	/// 弾を生成する関数。
	/// </summary>
	void BulletGenerate(IEnemy* e);
private:
	const int				IK_NUM = 2;					//IKの数。
	Quaternion				m_startRot = Quaternion::Identity;	//モデルが最初に向いている角度。
	float					m_sumLerp = 0.0f;
	//溜めビーム。
	Vector3					m_position = Vector3::Zero;
	float					m_chargeDamage = 0.f;
	float					m_chargeTimer = 0.f;
	float					m_endBeamTimer = 0.f;
	std::vector<Beam*>		m_beams;
	const float				m_chargeTime = 4.0f;
	bool					m_isSetPos = false;			//ビームを撃つ方向を設定。固定だから一度だけ設定する。
	IK*						m_chargebeamIk[2];			//IK。左右のIKの情報が格納されている。
	bool					m_isEndChargeBeam = false;	//溜めビームが終わったかどうか。
	float					m_sumMoveSpeed = 0.0f;
	bool					m_isKnockBackCB = false;
	bool					m_isChargeSound = false;
	CSoundSource*			m_chargeSE = nullptr;
	CSoundSource*			m_beamSE = nullptr;

	//乱射。
	float					m_shotTimer = 0.f;
	float					m_stateTimer = 0.f;
	float					m_shootingDamage = 0.f;
	IK*						m_shootingIk[2];			//IK。左右のIKの情報が格納されている。
	bool					m_isEndShooting = false;	//乱射が終わったかどうか。
	bool					m_isKnockBackSH = false;
	CSoundSource*			m_shootSE = nullptr;
};
