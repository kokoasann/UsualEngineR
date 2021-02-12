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

	void SetIEnemy(IEnemy* e) {
		m_IEnemy = e;
	}
	void SetState(const int& s) {
		m_state = s;
	}
private:
	IEnemy* m_IEnemy = nullptr;
	int m_state = 0;
	//saund
	CSoundSource*	m_bgm = nullptr;
	float			m_volume = 0.0f;
};
