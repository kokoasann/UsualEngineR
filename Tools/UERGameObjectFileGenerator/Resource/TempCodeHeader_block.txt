#pragma once

/// <summary>
/// 
/// </summary>
class XXX :public GameObject
{
public:
	XXX();
	virtual ~XXX();

	/// <summary>
	/// 本開放。確保したものを開放するための関数。
	/// </summary>
	virtual void Release() override;

	/// <summary>
	/// このオブジェクトの所持するゲームオブジェクト等を消すための関数。
	/// 面倒くさければRelease関数と同じでもよい。
	/// </summary>
	virtual void OnDestroy() override;

:block Awake
	/// <summary>
	/// NewGO時に即座に呼ばれる関数。
	/// コンストラクタの代わりに使おう。
	/// </summary>
	void Awake() override;
:end

:block Start
	/// <summary>
	/// スタート関数。
	/// </summary>
	/// <returns>trueを返さない限り、ずっとStartを呼び続ける</returns>
	bool Start() override;
:end

:block PreUpdate
	/// <summary>
	/// 更新。の前に呼ばれる更新。
	/// </summary>
	void PreUpdate() override;
:end
:block Update
	/// <summary>
	/// 更新。
	/// </summary>
	void Update() override;
:end
:block PostUpdate
	/// <summary>
	/// 更新。の後に呼ばれる更新。
	/// </summary>
	void PostUpdate() override;
:end

:block Render
	/// <summary>
	/// 描画。
	/// </summary>
	void Render() override;
:end

:block PostRender
	/// <summary>
	/// 手前に表示するやつの描画。
	/// </summary>
	void PostRender() override;
:end
private:

};