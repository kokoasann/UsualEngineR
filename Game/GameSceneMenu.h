#pragma once

/// <summary>
/// 
/// </summary>
class GameSceneMenu :public GameObject
{
public:
	GameSceneMenu();
	virtual ~GameSceneMenu();

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

	void PauseGame() {
		m_isPaused = true;
		DebugPrint_WATA("Game is paused\n");
	}

	void ResumeGame() {
		m_isPaused = false;
		DebugPrint_WATA("Game is resumed\n");
	}

	const bool IsGamePaused() {
		return m_isPaused;
	}

private:
	bool m_isPaused = false;

	bool m_isMenuActivated = false;

	enum class EnMenuButtonType {
		Restart,
		ToTitle,
		Close,
		NumType
	};

	int m_selectedTypeID = TO_INT(EnMenuButtonType::Close);

	//back sprite
	Sprite m_menuSprite;
	const Vector3 m_menuSpritePos = Vector3::Zero;
	const Vector3 m_menuSpriteScale = { 500.f, 500.f,1.f };
	const Vector4 m_spriteMulCol = { 0.f,0.f,0.f,0.4f };
	const float m_flSpHeight = 16.f;
	const float m_flSpWidth = 16.f;

	//font
	const float m_FontScale = 1.f;
	const Vector4 m_FontColor = Vector4::White;
	std::vector<Font> m_fonts;
	std::vector<std::wstring> m_texts;
};
