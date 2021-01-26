#pragma once

/// <summary>
/// 
/// </summary>
class Goal :public GameObject
{
public:
	Goal();
	virtual ~Goal();

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

	const Vector3& GetPosition() {
		return m_firstPosition;
	}

	const Quaternion& GetRotation() const{
		return m_rotation;
	}

private:

	//model
	ModelRender* m_model = nullptr;
	std::map<int, std::unique_ptr<CAnimationClip>> m_animationMap;

	//transform
	Vector3 m_firstPosition = { 0,-50.f,0 };
	Vector3 m_lastPosition = Vector3::Zero;
	Quaternion m_rotation = Quaternion::Identity;
	const Vector3 m_scale = { 100.f,100.f,100.f };

	//doorOpenPos
	const int CLOSED = 0;
	const int OPENED = 1;
	Vector3 m_openPosition = Vector3::Zero;
	const float m_openDoorSensorSensitivity = 15.f;
	const float m_openRange = 25.f;
	bool m_isOpened = false;

	const float m_range = 30.f;
	bool m_isChecked = false;

	float m_appearTimer = 0.f;
	const float m_appearTime = 3.0f;

	Vector3 m_forward = Vector3::One;

	//performance
	float m_performanceTimer = 0.f;
	const float m_sceneTransitionTime = 3.5f;
	//Physics
	PhysicsStaticObject m_physicsStaticObject;
};
