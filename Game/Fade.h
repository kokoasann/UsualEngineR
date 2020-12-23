#pragma once

/// <summary>
/// 
/// </summary>
class Fade :public GameObject
{
public:
	Fade();
	virtual ~Fade();

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

	void FadeIn() {
		m_state = State::enFadeIn;
		m_alpha = 1.f;
		m_isFaded = false;
	}

	void FadeOut() {
		m_state = State::enFadeOut;
		m_alpha = 0.f;
		m_isFaded = false;
	}

	const bool IsFaded() {
		return m_isFaded;
	}

private:
	enum class State {
		enFadeIn,
		enFadeOut,
		enIdle,
	};

	State m_state = State::enIdle;

	SpriteRender* m_panel = nullptr;
	const Vector3 m_panelScale = { 100,100,100 };
	const float m_panelHeight = 15;
	const float m_panelWidth = 300;

	float m_alpha = 1.f;
	float m_fadeSpeed = 1.f;

	bool m_isFaded = false;

};
