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
		return m_position;
	}

private:

	//model
	ModelRender* m_model = nullptr;

	//transform
	Vector3 m_position = { 0,-10.f,0 };
	Quaternion m_rotation = Quaternion::Identity;
	const Vector3 m_scale = { 10.5f,10.5f,10.5f };

	const float m_range = 30.f;
	bool m_isChecked = false;

	float m_appearTimer = 0.f;
	const float m_appearTime = 1.5f;

};
