#pragma once
#include "../../Player/State/IPlayerSstate.h"

/// <summary>
/// 
/// </summary>
class Projectile :public GameObject
{
public:
	Projectile();
	virtual ~Projectile();

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

	void Init(const Vector3& startPos, const Vector3 velocity) {
		m_position = startPos;
		m_velocity = velocity;
	}

	const Vector3& GetPosition() const {
		return m_position;
	}

private:

	//const
	const float m_lifeSpanSec = 10.f;
	const float m_speed = 100.f;
	const float m_range = 30.f;
	const float m_damage = 10 * 1.f / 5.f;


	float m_lifeTimerSec = 0.0f;

	//Model
	ModelRender* m_model = nullptr;
	Vector3 m_position = { 0,15,-20 };
	Vector3 m_velocity = { 0,0,0 };
	Quaternion m_rotation = Quaternion::Identity;
	const float m_scale = 1.f;

};
