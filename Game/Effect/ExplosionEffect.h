#pragma once
#include "Effect/ParticleEffect.h"
#include "Effect/VolumetricEffect.h"
/// <summary>
/// 
/// </summary>
class ExplosionEffect :public GameObject
{
public:
	ExplosionEffect();
	virtual ~ExplosionEffect();

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

	void SetPos(const Vector3& v)
	{
		m_effect->SetPos(v);
		//m_volumeEffect->SetPos({ v.x, v.y+m_volumeEffect->GetSca().y - m_volumeOffsetPos, v.z });
	}
	void SetSca(const Vector3& v)
	{
		m_effect->SetSca(v);
		//m_volumeEffect->SetSca()
	}

	void Play()
	{
		//m_isPlay = true;
		m_effect->SetGenerateFlag(true);
		//m_isTimeStop = false;
	}
	void Stop()
	{
		//m_isPlay = false;
		m_effect->SetGenerateFlag(false);
		//m_isTimeStop = false;
	}
private:
	PlaneParticleEffectRender* m_effect = nullptr;
	float m_particleTimer = 0.f;
	/*VolumetricEffectRender* m_volumeEffect = nullptr;
	bool m_isPlay = false;

	float m_timer = 0.f;
	float m_timeLimit = 3.f;
	bool m_isTimeStop = true;

	Vector3 m_volumeColor = { 0.05,0.05,0.05 };
	float m_volumeConcentration = 0.1f;

	float m_volumeOffsetPos = 20.f;
	float m_volumeOffsetCenterOffsetY = 50.f;*/
};
