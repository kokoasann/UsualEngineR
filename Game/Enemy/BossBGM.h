#pragma once
//#include "GameManager.h"
class IEnemy;


//class CSoundSource;
/// <summary>
/// 
/// </summary>
class BossBGM :public GameObject
{
public:
	BossBGM();
	virtual ~BossBGM();

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
	/// 更新。の前に呼ばれる更新。
	/// </summary>
	void PreUpdate() override;
	/// <summary>
	/// 更新。
	/// </summary>
	void Update() override;

	
	void AddBgmCount(int count) {
		m_bgmCount += count;
		m_bgmCount = max(0.f, m_bgmCount);
	}
	int GetBgmCount() {
		return m_bgmCount;
	}
private:

	//saund
	CSoundSource*	m_bgm = nullptr;
	float			m_volume = 0.0f;
	int m_bgmCount = 0;

};
