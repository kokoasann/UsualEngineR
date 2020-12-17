#pragma once
#include "Effect/ParticleEffect.h"

struct BeamEffectInitData
{
	float particleLifeTime = 0.07f;
	float particleScale = 1.f;
	Vector4 particleColor = { 2.f,3.f,5.f,1.f };
	//Vector4 particleEndColor = { 0.5f,0.25f,0.05f,0.f };

	int circleNum = 4;					//円のパーティクルの数.
	int circleStrokeNum = 10;			//縦の伸びのパーティクルの数。多ければ多いほど長く伸びる.
	float circleSize = 0.1f;			//円の大きさ.
	Vector3 velocity = { 1.f,5.f,0.f };	//xがへの広がり、yが縦へ.
};
/// <summary>
/// 
/// </summary>
class Beam :public GameObject
{
public:
	Beam();
	virtual ~Beam();

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

	void Init(const BeamEffectInitData& bid);

	void Play();

	void Stop();

	void SetPos(Vector3 pos)
	{
		pos.y += 4.f;
		m_position = pos;
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
	void SetDir(const Vector3& dir)
	{
		m_direction = dir;
		m_direction.Normalize();
	}
private:
	PlaneParticleEffectRender*	m_effect;
	Vector3						m_position = Vector3::Zero;
	Vector3						m_direction = Vector3::Zero;
	float						m_particleScale = 1.f;
	Vector4						m_particleColor = { 0.f,0.f,5.f,1.f };
	float						m_particleLifeTime = 5.f;

	bool						m_isPlay = false;
};
