#pragma once
#include "Effect/ParticleEffect.h"
class JetEffect;

/// <summary>
/// 
/// </summary>
class JetPack :public GameObject
{
public:
	JetPack();
	virtual ~JetPack();

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

	void SetPosition(const Vector3& pos) {
		m_position = pos;
	}

	void SetRotation(const Quaternion& rot) {
		m_rotation = rot;
	}

	void SetUsingState(const bool flag) {
		m_isUsed = flag;
	}

	void FireThrusters() {
		m_isUsingThrusters = true;
	}

	void StopThrusters() {
		m_isUsingThrusters = false;
	}

	const double GetActivatedTime() {
		return m_activeTimer;
	}

private:

	//Model
	ModelRender* m_model = nullptr;
	Vector3 m_position = { 0,15,-20 };
	Quaternion m_rotation = Quaternion::Identity;
	const Vector3 m_scale = { 0.5f,0.5f,0.5f };

	bool m_isUsed = true;

	//effect
	bool m_isUsingThrusters = false;
	//PlaneParticleEffectRender* m_effect;
	//PlaneParticleEffectRender* m_effect_2;
	//PlaneParticleEffectRender* m_effects[4];
	Bone* m_backpackBone[6] = { nullptr };
	std::vector<JetEffect*> m_jetEffects;

	//timer
	double m_activeTimer = 0.f;
};
