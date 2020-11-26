#pragma once
#include "Effect/ParticleEffect.h"


struct MuzzleFlashEffectInitData
{
	float particleLifeTime = 0.07f;
	float particleScale = 1.f;
	Vector4 particleColor = { 4.f,2.f,0.4f,1.f };
	Vector4 particleEndColor = { 0.5f,0.25f,0.05f,0.f };

	int circleNum = 4;					//円のパーティクルの数.
	int circleStrokeNum = 10;			//縦の伸びのパーティクルの数。多ければ多いほど長く伸びる.
	float circleSize = 0.1f;			//円の大きさ.
	Vector3 velocity = { 1.f,5.f,0.f };	//xがへの広がり、yが縦へ.
};
/// <summary>
/// 
/// </summary>
class MuzzleFlash :public GameObject
{
public:
	

	MuzzleFlash();
	virtual ~MuzzleFlash();

	/// <summary>
	/// 本開放。確保したものを開放するための関数。
	/// </summary>
	virtual void Release() override;

	/// <summary>
	/// このオブジェクトの所持するゲームオブジェクト等を消すための関数。
	/// 面倒くさければRelease関数と同じでもよい。
	/// </summary>
	virtual void OnDestroy() override;


	void Init(const MuzzleFlashEffectInitData& mfid);

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
	PlaneParticleEffectRender* m_effect;

	float m_particleLifeTime = 0.07f;
	float m_particleScale = 1.f;
	Vector4 m_particleColor = { 2.f,1.f,0.4f,1.f };
	Vector4 m_particleEndColor = { 0.5f,0.25f,0.05f,0.f };

	int m_circleNum = 4;
	int m_circleStrokeNum = 10;
	float m_circleSize = 0.1f;
	Vector3 m_velocity = { 1.f,5.f,0.f };

	bool m_isPlay = false;
};
