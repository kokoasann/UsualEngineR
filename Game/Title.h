#pragma once

/// <summary>
/// 
/// </summary>
class Title :public GameObject
{
public:
	Title();
	virtual ~Title();

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
private:
	void LoadSoundSources();
	//Back Sprite
	SpriteRender* m_backSprite = nullptr;
	SpriteRender* m_forwardSprite = nullptr;
	Vector3 m_backSpPos = Vector3::Zero;
	bool m_isTrigeredStart = false;
	static bool m_seLoaded;

	float m_timer = 0;
	float m_colorTimer = 0.f;
	float m_colorTimer_max = 1.f;
	bool m_isAddColor = true;
	Font m_font;
};
