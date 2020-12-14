#pragma once

/// <summary>
/// 
/// </summary>
class TargetMarker :public GameObject
{
public:
	TargetMarker();
	virtual ~TargetMarker();

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

	/// <summary>
/// 親オブジェクトの座標を指定
/// </summary>
/// <param name="pos"></param>
	void SetParentPos(const Vector3& pos) {
		m_parentPos = pos;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="flag">draw flag</param>
	void SetDrawFlag(const bool flag) {
		m_spMarker->SetActive(flag);
	}

	void SetMarkerPosOffset(const Vector3& offset) {
		m_markerPosOffset = offset;
	}

private:
	bool m_shouldBeDrawn = false;
	Vector3 m_parentPos = Vector3::Zero;
	SpriteRender* m_spMarker = nullptr;
	const float m_flSpHpHeight = 32;
	const float m_flSpHpWidth = 32;
	const Vector3 m_markerScale = { 1.f / 200.f , 1.f / 200.f,1 };
	Vector3 m_markerPosOffset = { 0.f,0.f,0.f };
	const Vector2 m_SPRITE_PIVOT = { 1.f,0.f };
	const Vector4 m_COLOR = { 1,1,1,1 };
};
