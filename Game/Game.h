#pragma once
class IEnemy;

/// <summary>
/// ゲーム
/// </summary>
class Game :public GameObject
{
public:
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

	void OnEnterBattle(IEnemy* enemy);

	void OnEnemyDied(IEnemy* enemy);

	void OnItemUnlocked();

	void EndBossPerform()
	{
		m_isBossCamPerform = false;
	}

	void OnGoal();

	void GoalGatePerformance();

	void ToTitle();

	void Restart();

private:
	bool m_isCreateEnemyManager = false;		//EnemyManagerを作成したかどうか。
	bool m_isBossCamPerform = false;
	IEnemy* m_boss = nullptr;
	bool m_isCalledExplode = false ;
	float m_timer = 0.f;

	bool m_isGenerateGoalAfterBossPerformance = false;
	float m_goalAppearTimer = 0.f;
	float m_goalAppearTime = 2.8f;

	double m_clearTimer = 0.f;

	bool m_isCleared = false;

	bool m_restartFlag = false;
	bool m_toTitleFlag = false;
};