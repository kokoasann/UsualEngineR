#pragma once
class ExplosionEffect;
/// <summary>
/// 
/// </summary>
class Enemy_LaunchBomb :public GameObject
{
public:
	Enemy_LaunchBomb();
	virtual ~Enemy_LaunchBomb();

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
	/// 
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="fireVelo">出る方向</param>
	/// <param name="size">弾のサイズの半径</param>
	/// <param name="fireRange">攻撃判定の半径</param>
	/// <param name="damage"></param>
	/// <param name="power">吹っ飛びの強さ</param>
	void Init(const Vector3& pos,const Vector3& fireVelo,float size,float fireRange,float damage,float power);

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


private:
	ModelRender* m_model;
	SphereCollider m_coll;
	SphereCollider m_fireColl;
	RigidBody m_rigidBody;
	GhostObject m_ghost;
	Vector3 m_velo = Vector3::Zero;
	ExplosionEffect* m_effect;

	bool m_isDestroy = false;
	bool m_isHit = false;
	Vector3 m_hitPos = Vector3::Zero;

	float m_fireRange = 0.f;
	float m_damage = 0.f;
	float m_power = 0.f;
};
