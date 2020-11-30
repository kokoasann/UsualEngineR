#pragma once
#include "CCameraCollisionSlover.h"
#include "SpringCamera.h"
#include <tuple>

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

	const float m_swivelSpeed = 150.f;
	const float m_transitionSpeed = 2.f;
	const Vector3 m_furtherTargetHeight = { 0.f,12.f,0.f };
	Vector3 m_position = { 0,3,-60 };
	Vector3 m_old = { 0,3,15 };
	Vector3 m_dist = Vector3::Zero;
	Vector3 m_targetPos = Vector3::Zero;
	Vector3 m_charaPos = Vector3::Zero;
	Quaternion m_rot = Quaternion::Identity;

	void UpdateState();
	void CalcTarget();
	void CalcEnemyCamera();
	void CalcPlayerCamera();

	/// <summary>
	/// カメラのターゲットになる敵を計算. (存在しない場合は-1を返す).
	/// </summary>
	/// <returns>カメラ中央に見える敵のインデックス, カメラの左側に見える敵のインデックス, カメラの右側に見える敵のインデックス</returns>
	std::tuple<int, int, int> GetTargetEnemyIndexes();

	Vector3 m_playerCameraPos = m_position;
	Vector3 m_playerCameraPreviousPos = m_position;
	Vector3 m_playerCameraTargetPos = Vector3::Zero;

	Vector3 m_enemyCameraPos = m_position;
	Vector3 m_enemyCameraTargetPos = Vector3::Zero;

	//test
	Vector3 m_enemyCameraNextTargetPos = Vector3::Zero;
	Vector3 m_enemyCameraCurrentTargetPos = Vector3::Zero;

	Player* mp_player = nullptr;
	Vector3 m_toCameraPos;

	float m_cameraChangeRatio = 1.f;

	//Physics
	//SpringCamera m_spc;
	CCameraCollisionSlover m_cameraCollisionSolver;
	const float m_sphereCollisionRadius = 0.5f;

	float enemyTargetChangeTime = 0.f;
	PositionChecker* m_posChecker = nullptr;
	PositionChecker* m_posCheckerL = nullptr;
	PositionChecker* m_posCheckerR = nullptr;
	int m_targetEnemyNo = 0;

	//カメラの急な動きを防ぐ用.
	float m_oldDist = 100.f;
	bool m_isInitedInitialDist = false;
};

static const void DebugLogVec3(const Vector3& vec) {
	printf("Vec X : %f / Vec Y : %f / Vec Z : %f\n", vec.x, vec.y, vec.z);
}
