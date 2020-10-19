#pragma once

/// <summary>
/// 
/// </summary>
class HealthBar :public GameObject
{
public:
	HealthBar();
	virtual ~HealthBar();

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
	/// 現在の体力の割合をセット
	/// </summary>
	/// <param name="scale">CurrentHP / Max HP</param>
	void SetHealthRate(const float rate) {
		m_currentHpScale.x = m_hpScale.x * rate;
	}

	/// <summary>
	/// 指定した秒数ヘルスバーを表示
	/// </summary>
	void ShowHealthBar() {
		m_spHp->SetActive(true);
		m_shouldBeDrawn = true;
		m_drawTimer = 0.f;
		printf("should be drawn\n");
	}

private:
	bool m_shouldBeDrawn = false;
	float m_drawTimer = 0.f;
	const float m_drawSec = 1.5f;
	Vector3 m_parentPos = Vector3::Zero;
	SpriteRender* m_spHp = nullptr;
	const float m_flSpHpHeight = 16;
	const float m_flSpHpWidth = 16;
	const Vector3 m_hpScale = { 1.f / 80.f , 1.f / 800.f,1 };
	Vector3 m_currentHpScale = m_hpScale;
	Vector3 m_hpPosOffset = { 0.f,10.f,0.f };
	const Vector2 m_SPRITE_PIVOT = { 1.f,0.f };
};
