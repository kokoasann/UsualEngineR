#pragma once

/// <summary>
/// 
/// </summary>
class PositionChecker :public GameObject
{
public:
	PositionChecker();
	virtual ~PositionChecker();

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
	/// スタート関数。
	/// </summary>
	/// <returns>trueを返さない限り、ずっとStartを呼び続ける</returns>
	bool Start() override;

	/// <summary>
	/// 更新。
	/// </summary>
	void Update() override;

	const Vector3& GetPos() const {
		return m_position;
	}

	void SetPos(const Vector3& pos) {
		m_position = pos;
	}
private:
	ModelRender* m_model = nullptr;
	Vector3 m_position = { 0,0,0 };
	Vector3 m_scale = { 2.f,200.f,2.f };
};
