#pragma once
class Player;
class IEnemy;
class PositionChecker;

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

	void SetEnemy(IEnemy* enemy) {
		mp_enemy = enemy;
	}

	void SetPlayer(Player* player) {
		mp_player = player;
	}

	bool IsTargettingEnemy() {
		return m_state == State::enEnemyCamera;
	}

private:


	enum class State{
		enEnemyCamera,
		enPlayerCamera,
		enNumState
	};
	
	State m_state = State::enPlayerCamera;

	bool isChanging = false;

	const float m_swivelSpeed = 60.f;
	const float m_transitionSpeed = 0.02f;
	const Vector3 m_furtherTargetHeight = { 0.f,6.f,0.f };
	Vector3 m_position = { 0,3,15 };
	Vector3 m_old = { 0,3,15 };
	Vector3 m_dist = Vector3::Zero;
	Vector3 m_targetPos = Vector3::Zero;
	Vector3 m_charaPos = Vector3::Zero;
	Quaternion m_rot = Quaternion::Identity;

	void UpdateState();

	void CalcEnemyCamera();
	void CalcPlayerCamera();

	Vector3 m_playerCameraPos = m_position;
	Vector3 m_playerCameraTargetPos = Vector3::Zero;

	Vector3 m_enemyCameraPos = m_position;
	Vector3 m_enemyCameraTargetPos = Vector3::Zero;

	Player* mp_player = nullptr;
	IEnemy* mp_enemy = nullptr;

	Vector3 m_toCameraPos;
	//SpringCamera m_springCamera;

	//PositionChecker* m_posChecker = nullptr;

	//std::vector< PositionChecker*> m_posCheckers;

	float m_cameraChangeRatio = 0.f;
};

static const void DebugLogVec3(const Vector3& vec) {
	printf("Vec X : %f / Vec Y : %f / Vec Z : %f\n", vec.x, vec.y, vec.z);
}
