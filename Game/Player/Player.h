#pragma once
#include "IPlayerSstate.h"
#include "../../UsualEngineR/Character/CharacterController.h"

/// <summary>
/// 
/// </summary>
class Player :public GameObject
{
public:

	enum EnState {
		enGround,
		enFlying,
		enNumState
	};

	Player();
	virtual ~Player();

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

	IPlayerState* GetState(int state) {
		return m_stateList[state];
	};

	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <returns>座標</returns>
	const Vector3& GetPosition() const {
		return m_position;
	}

	void SetPosition(Vector3& pos) {
		m_position = pos;
	}

	void SetVelocity(Vector3& velocity) {
		m_velocity = velocity;
	}

	/// <summary>
	/// プレイヤーの回転を取得
	/// </summary>
	/// <returns>プレイヤーに適応されるクォータニオン</returns>
	const Quaternion& GetRotation() const {
		return m_rotation;
	}

	void SetRotation(Quaternion& rot) {
		m_rotation = rot;
	}

	const float GetSpeed() const {
		return m_speed;
	}

private:
	//State
	IPlayerState* m_nextState = nullptr;
	IPlayerState* m_currentState = nullptr;
	std::vector<IPlayerState*> m_stateList;

	//Model
	ModelRender* m_model = nullptr;
	CAnimationClip m_animClip[1];
	std::vector<CAnimationClipPtr> m_animlist;
	Vector3 m_position = { 0,5,0 };
	Quaternion m_rotation = Quaternion::Identity;
	const float m_scale = 0.05f;

	//Move
	CharacterController m_charaCon;
	Vector3 m_velocity = Vector3::Zero;
	float m_speed = 300.f;

	//Physics
	float m_charaConRadius = 5.f;
	float m_charaConHeight = 15.f;
};