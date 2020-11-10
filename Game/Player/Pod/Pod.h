#pragma once
#include "../Attack/PlayerLongRangeAttack.h"

class Player;

/// <summary>
/// 
/// </summary>
class Pod :public GameObject
{
public:
	Pod();
	virtual ~Pod();

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

	void SetPlayer(Player* player) {
		mp_player = player;
	}

	void SetPosition(const Vector3& pos) {
		m_pos = pos;
	}

	const Vector3& GetPosition() const {
		return m_pos;
	}

	/// <summary>
	/// Podを投げる
	/// </summary>
	/// <param name="velocity">Velocity</param>
	/// <param name="thrownTime">何秒間投げるか</param>
	void Thrown(const Vector3& velocity, const float thrownTime = 1.f) {
		m_velocity = velocity;
		m_thrownTime = thrownTime;
		m_state = PodState::enThrown;
		m_thrownTimer = 0.f;
	}

private:

	void ShotLaserBeam();
	void ThrownBehave();
	void Rampage();
	void Kamikaze();

	//model
	ModelRender* m_model = nullptr;

	//transform
	Player* mp_player = nullptr;
	Vector3 m_pos = Vector3::Zero;
	Vector3 m_distanceFromPlayer = { 5.f,5.f,5.f };
	Quaternion m_rotation = Quaternion::Identity;
	const Vector3 m_scale = { 0.5f,0.5f,0.5f };


	float m_timer = 0.f;

	//Thrown
	Vector3 m_velocity = Vector3::Zero;
	float m_thrownTime = 1.f;
	const float m_thrownAttackDamageAmount = 20.f;
	const float m_thrownAttackRange = 30.f;

	//Rampage
	const float m_rampageTime = 5.f;
	const float m_rampagingDamageAmount = 100.f;

	//Kamikaze
	const float m_kamikazeDamageAmount = 300.f;

	//State
	enum class PodState {
		enIdle,
		enThrown,
		enRampage,
		enKamikaze,
		enNumPodState
	};

	PodState m_state = PodState::enIdle;

	PlayerLongRangeAttack m_longRangeAttack;

};
