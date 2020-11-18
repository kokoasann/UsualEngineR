#pragma once


/// <summary>
/// 
/// </summary>
class Enemy_Bullet final:public GameObject
{
public:
	Enemy_Bullet();
	virtual ~Enemy_Bullet();

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
	/// 初期化。
	/// dirはNomalizeしておくこと。
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="scale"></param>
	/// <param name="dir"></param>
	/// <param name="speed"></param>
	void Init(const Vector3& pos, float scale, const Vector3& dir, float speed, float timeLimit, float attackRenge);

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


	void SetDir(const Vector3& v)
	{
		m_dir = v;
	}
	const Vector3& GetDir() const
	{
		return m_dir;
	}

	void SetSpeed(const float f)
	{
		m_speed = f;
	}
	float GetSpeed() const
	{
		return m_speed;
	}

	void SetDamage(const float f)
	{
		m_damage = f;
	}
private:
	ModelRender* m_model;
	float m_scale;
	
	Vector3 m_dir;
	float m_speed;

	float m_timer = 0.f;
	float m_timeLimit = 0.f;

	float m_attackRange = 0.f;
	float m_damage = 100.f;

	SphereCollider m_sphere;
};
