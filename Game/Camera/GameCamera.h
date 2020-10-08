#pragma once
/// <summary>
/// 
/// </summary>
class GameCamera :public GameObject
{
public:
	GameCamera();
	virtual ~GameCamera();

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

	void PostUpdate() override;

	void SetTarget(const Vector3& target) {
		m_targetPos = target;
	}

	void SetCharaPos(const Vector3& charaPos)
	{
		m_charaPos = charaPos;
	}

private:

	enum class State {
		enEnemyCamera,
		enPlayerCamera,
		enNumState
	};
	
	State m_state = State::enEnemyCamera;

	const float m_swivelSpeed = 60.f;
	Vector3 m_position = { 0,3,15 };
	Vector3 m_old = { 0,3,15 };
	Vector3 m_dist = Vector3::Zero;
	Vector3 m_targetPos = Vector3::Zero;
	Vector3 m_charaPos = Vector3::Zero;
	Quaternion m_rot = Quaternion::Identity;
};

static const void DebugLogVec3(const Vector3& vec) {
	printf("Vec X : %f / Vec Y : %f / Vec Z : %f\n", vec.x, vec.y, vec.z);
}
