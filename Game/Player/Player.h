#pragma once
#include "IPlayerSstate.h"

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
private:
	//State
	IPlayerState* m_nextState = nullptr;
	IPlayerState* m_currentState = nullptr;
	std::vector<IPlayerState*> m_stateList;

	//Model
	ModelRender* m_model = nullptr;
	CAnimationClip m_animClip[1];
	std::vector<CAnimationClipPtr> m_animlist;
	Vector3 m_position = Vector3::Zero;
};