#pragma once
#include "Effect/ParticleEffect.h"

struct BeamEffectInitData
{
	float particleLifeTime = 0.35f;
	float particleScale = 1.f;
	Vector4 chargeColor = { 1.f,2.f,5.f,1.f };
	Vector4 beamColor = { 2.f,3.f,5.f,1.f };
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
		/*const float yUp = 4.f;
		pos.y += yUp;*/
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
	void SetToPlayerDir(const Vector3& dir)
	{
		m_direction = dir;
		m_direction.Normalize();
	}
	void SetHolizontalDir(const Vector3& hdir)
	{
		m_holizontalDir = hdir;
		m_holizontalDir.Normalize();
	}
	void SetChange(const bool isChange) 
	{
		m_isCharge = isChange;
	}
	void SetIsLoop(const bool isLoop) 
	{
		m_isLoop = isLoop;
	}
	
private:
	PlaneParticleEffectRender*	m_effect = nullptr;
	Vector3						m_position = Vector3::Zero;
	Vector3						m_holizontalDir = Vector3::AxisX;
	Vector3						m_direction = Vector3::AxisY;
	float						m_particleScale = 1.f;
	Vector4						m_chargeColor = { 0.f,0.f,5.f,1.f };
	Vector4						m_beamColor = { 0.f,0.f,5.f,1.f };
	float						m_particleLifeTime = 0.f;

	bool						m_isPlay = false;
	bool						m_isCharge = false;
	bool						m_isLoop = false;
};
