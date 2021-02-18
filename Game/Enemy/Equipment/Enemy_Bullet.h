#pragma once

class Ballistic;

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

	void SetDamage(const float f, const bool stunFlag = false, const Vector3& vel = Vector3::Zero)
	{
		m_damage = f;
		m_isStunFlag = stunFlag;
		m_vel = vel;
	}

private:
	//ModelRender* m_model;
	float m_scale;
	
	Vector3 m_dir;
	float m_speed;

	float m_timer = 0.f;
	float m_timeLimit = 0.f;

	float m_attackRange = 0.f;
	float m_damage = 10.f;
	bool  m_isStunFlag = false;
	Vector3 m_vel = Vector3::Zero;

	SphereCollider m_sphere;

	Vector3 m_pos;
	Quaternion m_rot;
	Vector3 m_sca;

	int m_instanceNum = 0;
	Matrix* m_worldMatrix = nullptr;
	//Ballistic* m_effect;
};


class EnemyBulletManager final : public GameObject
{
public:
	static EnemyBulletManager* GetInstance();
	
	void Release() override;
	void OnDestroy() override;

	void Awake() override;

	void PostUpdate() override;

	void Allocate(int num);

	std::pair<int, Matrix*> GetInstanceMatrix();

	void AddInstance();
	void DeadInstance(int num);

	void InstanceNULL()
	{
		for (int i=0;i<m_usingMatrix.size();i++)
		{
			m_usingMatrix[i] = false;
		}
		m_model->SetInstanceNum(0);
	}
private:
	StructuredBuffer m_structuredBuff;
	std::vector<Matrix> m_instanceMatrix;
	std::vector<bool> m_usingMatrix;
	ModelRender* m_model = 0;

	std::mutex m_mutex;

	std::vector<Enemy_Bullet*> m_bulletList;
	int m_bulletNum = 0;
	int m_nextAllocateActiveNum = 30;
	int m_allocElementNum = 60;
	ThreadObject m_thread;

	bool m_isAllocate = false;
	int m_oldNum = 0;

	//bool m_isInit = false;
};

extern EnemyBulletManager* g_enemyBulletManager;