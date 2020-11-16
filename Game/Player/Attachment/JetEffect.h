#pragma once
#include "Effect/ParticleEffect.h"

/// <summary>
/// 
/// </summary>
class JetEffect :public GameObject
{
public:
	JetEffect();
	virtual ~JetEffect();

	struct JetEffectInitParam {
		float effectScale;
		float effectScale_inv;
		float particleScale;
		float particleLifeTime;
		float particleYUp;
	};

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
	/// 更新。の前に呼ばれる更新。
	/// </summary>
	void PreUpdate() override;
	/// <summary>
	/// 更新。
	/// </summary>
	void Update() override;
	/// <summary>
	/// 更新。の後に呼ばれる更新。
	/// </summary>
	void PostUpdate() override;

	/// <summary>
	/// 描画。
	/// </summary>
	void Render() override;

	/// <summary>
	/// 手前に表示するやつの描画。
	/// </summary>
	void PostRender() override;

	void Init(const JetEffectInitParam&);

	void SetPosition(const Vector3& pos) {
		m_pos = pos;
	}

	void SetRotation(const Quaternion& rot) {
		m_rot = rot;
	}

	void SetGenerateFlag(const bool generateFlag) {
		m_particleEffect->SetGenerateFlag(generateFlag);
	}

private:
	PlaneParticleEffectRender* m_particleEffect = nullptr;
	Vector3 m_pos = Vector3::Zero;
	Quaternion m_rot = Quaternion::Identity;
	Vector3 m_oldPos = Vector3::Zero;
};