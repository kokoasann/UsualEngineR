#pragma once
class SmokeEffect;

/// <summary>
/// 
/// </summary>
class Shield :public GameObject
{
public:
	Shield();
	virtual ~Shield();

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

	void SetPosition(const Vector3& pos) {
		m_position = pos;
	}

	void SetRotation(const Quaternion& rot) {
		m_rotation = rot;
	}

	void SetUsingState(const bool flag) {
		m_isUsed = flag;
	}

private:

	//Model
	ModelRender* m_model = nullptr;
	Vector3 m_position = { 0,15,-20 };
	Quaternion m_rotation = Quaternion::Identity;
	const Vector3 m_scale = { 0.5f,0.5f,0.5f };

	bool m_isUsed = true;

	std::vector<SmokeEffect*> m_worldSmokeEffects;
	std::vector<SmokeEffect*> m_localSmokeEffects;

	enum class ShieldBone {
		Top,
		Left,
		Right
	};

	Bone* m_shieldBones[3] = { nullptr };

	Vector3 m_smokeEffectScale = Vector3::One * 0.02;
};
