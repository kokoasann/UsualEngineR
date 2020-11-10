#pragma once
#include "../../Player/Player.h"

/// <summary>
/// 
/// </summary>
class DropItemA :public GameObject
{
public:
	DropItemA();
	virtual ~DropItemA();

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

	void SetPosition(const Vector3& pos) {
		m_position = pos;
	}

private:

	//const
	const float m_GRASP_RANGE = 20.f;
	const Player::EnAttackType m_typeId = Player::EnAttackType::enRemoteAttackPreset;

	//Model
	ModelRender* m_model = nullptr;
	Vector3 m_position = { 0,15,-20 };
	Quaternion m_rotation = Quaternion::Identity;
	const float m_scale = 1.f;

};
