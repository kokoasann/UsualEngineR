#pragma once
class Beam;
class MuzzleFlash;
/// <summary>
/// 
/// </summary>
class Boss_FatmanAngryPerform :public GameObject
{
public:
	Boss_FatmanAngryPerform();
	virtual ~Boss_FatmanAngryPerform();

	/// <summary>
	/// 本開放。確保したものを開放するための関数。
	/// </summary>
	virtual void Release() override;

	/// <summary>
	/// このオブジェクトの所持するゲームオブジェクト等を消すための関数。
	/// 面倒くさければRelease関数と同じでもよい。
	/// </summary>
	virtual void OnDestroy() override;

	/// <summary>
	/// NewGO時に即座に呼ばれる関数。
	/// コンストラクタの代わりに使おう。
	/// </summary>
	void Awake() override;

	/// <summary>
	/// スタート関数。
	/// </summary>
	/// <returns>trueを返さない限り、ずっとStartを呼び続ける</returns>
	bool Start() override;

	/// <summary>
	/// 更新。
	/// </summary>
	void Update() override;

	void SetBeamIk(IK** b) {
		
			m_BeamIk = b;
	}
	void SetShootIk(IK** b) {
		m_ShootIk = b;
	}

private:
	const int IK_NUM = 2;

	bool m_isPerformed = false;
	IK** m_BeamIk = nullptr;
	std::vector< Beam*> m_beams;
	std::vector< MuzzleFlash*> m_muzzleFlashes;
	IK** m_ShootIk = nullptr;

};
