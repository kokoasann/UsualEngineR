#pragma once

/// <summary>
/// 
/// </summary>
class KeyHelp :public GameObject
{
public:
	KeyHelp();
	virtual ~KeyHelp();

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

	void Init(const Vector3& pos, const std::wstring text);

	void Display();
	void Undisplay();


	void SetPos(const Vector3& pos)
	{
		m_pos = pos;
		m_keyHelpSp.Update(m_pos + m_spPosOffset, Quaternion::Identity, m_spScale);
	}
private:
	Vector3 m_pos = Vector3::Zero;
	const float m_rot = 0.f;

	Sprite m_keyHelpSp;
	const Vector3 m_spScale = Vector3::One * 0.25;
	const Vector3 m_spPosOffset = Vector3(-100.f, -13.f, 0.f);
	const float m_flSpHeight = 100.f;
	const float m_flSpWidth = 500.f;

	Font m_keyHelpText;
	const float m_fontScale = 0.2f;
	Vector4 m_color = Vector4::White;
	std::wstring m_text;
	const Vector2 m_fontPosOffset = Vector2(-150.f, 0.f);


	float m_timer = 0.f;
	float m_dispTime = 0.2f;

	bool m_isDisp = false;
};
