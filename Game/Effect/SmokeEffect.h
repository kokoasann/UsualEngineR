#pragma once
#include "Effect/ParticleEffect.h"

/// <summary>
/// 
/// </summary>
class SmokeEffect :public GameObject
{
public:
	SmokeEffect();
	virtual ~SmokeEffect();

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

	void Init();

	void SetPos(const Vector3& v)
	{
		m_effect->SetPos(v);
	}
	void SetSca(const Vector3& v)
	{
		m_effect->SetSca(v);
	}

	void Play()
	{
		m_effect->SetGenerateFlag(true);
	}
	void Stop()
	{
		m_effect->SetGenerateFlag(false);
	}

private:
	PlaneParticleEffectRender* m_effect = nullptr;
	float m_particleTimer = 0.f;
};
