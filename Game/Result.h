#pragma once
#include "level/Level.h"

/// <summary>
/// 
/// </summary>
class Result :public GameObject
{
public:
	Result();
	virtual ~Result();

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

	void Init(const double clearTime);

private:

	float m_clearTime = 0.f;
	std::vector<ModelRender*> m_robots;
	const Vector3 m_scale = { 1.5f,1.5f,1.5f };
	int m_numRobot = 50;

	ModelRender* m_ground = nullptr;

	const Vector3 m_resultCameraPos = Vector3(0.f,9.f, 8.2f);
	const Vector3 m_resultCameraTarget = Vector3(0.f,5.f,0.f);

	//Back Sprite
	SpriteRender* m_backSprite = nullptr;
	Vector3 m_backSpPos = Vector3(0.f, 220.f, 0.f);
	Vector3 m_backSpScale = Vector3(5.5f, 1.f, 1.f);

	Level m_level;
	const float m_levelScale = 1.5f;

	bool m_isTriggeredButtonA = false;
};
