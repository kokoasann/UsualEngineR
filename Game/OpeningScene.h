#pragma once
#include <unordered_map>
class EventMovie;

/// <summary>
/// 
/// </summary>
class OpeningScene :public GameObject
{
public:
	OpeningScene();
	virtual ~OpeningScene();

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

	template<class T, class U>
	bool contain(const std::basic_string<T>& s, const U& v) {
		return s.find(v) != std::basic_string<T>::npos;
	}

	ModelRender* CreateModelRender(const std::string& name);
	EventMovie* m_eventMovie = nullptr;
	std::vector<ModelRender*> m_modelRenders;//まとめて解放する用。

	ModelRender* m_chara = nullptr;
	ModelRender* m_bomb = nullptr;
	ModelRender* m_melee = nullptr;
	ModelRender* m_fat = nullptr;
	std::vector<ModelRender*> m_pods;
};
