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

	void Init(const Vector3& pos, float scale, const Vector3& dir, float speed, float timeLimit, float attackRenge);


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

	float m_lifeSpanSec = 10.f;
	 float m_speed = 100.f;
	float m_range = 30.f;
	const float m_damage = 10 *1.f / 5.f;


	float m_lifeTimerSec = 0.0f;

	//Model
	ModelRender* m_model = nullptr;
	Vector3 m_position = { 0,15,-20 };
	Vector3 m_velocity = { 0,0,0 };
	Quaternion m_rotation = Quaternion::Identity;
	//const float m_scale = 1.f;


	//
	float m_scale;
	Vector3 m_dir;

	//float m_timer = 0.f;
	//float m_timeLimit = 0.f;

	SphereCollider m_sphere;
};


/// ///////////////////////

class PlayerBulletManager final : public GameObject
{
public:
	void Release() override;
	void OnDestroy() override;

	void PostUpdate() override;

	void Allocate(int num);
private:
	std::mutex m_mutex;

	std::vector<Projectile*> m_bulletList;
	int m_bulletNum = 0;
	int m_nextAllocateActiveNum = 6;
	int m_allocElementNum = 30;
	ThreadObject m_thread;

	bool m_isAllocate = false;
	int m_oldNum = 0;
};