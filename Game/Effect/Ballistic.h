#pragma once
#include "Effect/ParticleEffect.h"

/// <summary>
/// 
/// </summary>
class Ballistic :public GameObject
{
public:
	Ballistic();
	virtual ~Ballistic();

	/// <summary>
	/// 本開放。確保したものを開放するための関数。
	/// </summary>
	virtual void Release() override;

	/// <summary>
	/// このオブジェクトの所持するゲームオブジェクト等を消すための関数。
	/// 面倒くさければRelease関数と同じでもよい。
	/// </summary>
	virtual void OnDestroy() override;

	void Init();

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

	void Play();

	void Stop();


	void SetPos(const Vector3& pos)
	{
		m_effect->SetPos(pos);
	}
	void SetRot(const Quaternion& rot)
	{
		m_effect->SetRot(rot);
	}
	void SetSca(const Vector3& sca)
	{
		m_effect->SetSca(sca);
	}


private:
	PlaneParticleEffectRender* m_effect = nullptr;

	float m_particleLifeTime = 2.f;
	float m_particleScale = 0.05f;
	Vector4 m_particleColor = { 0.8f,0.8f,0.8f,0.7f };
	Vector4 m_particleEndColor = { 0.8f,0.8f,0.8f,0.f };

	int m_circleNum = 4;
	int m_circleStrokeNum = 10;
	float m_circleSize = 0.1f;

	bool m_isPlay = false;
};
