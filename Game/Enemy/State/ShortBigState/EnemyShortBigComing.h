#pragma once
#include "Enemy/State/IEnemyState.h"

class EnemyShortBigComing final : public IEnemyState
{
public:
	EnemyShortBigComing();
	~EnemyShortBigComing();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
	
private:
	/// <summary>
	/// 移動
	/// </summary>
	/// <param name="e"></param>
	void Move(IEnemy* e);
	/// <summary>
	/// アニメーション
	/// </summary>
	/// <returns>アニメーションが終わったかどうかを返す。</returns>
	bool IsAnimation();
private:
	float m_danceTimer = 0.f;	//ダンス行動をするかどうかの判定を行うまでの時間計測。

	IK* m_headIK = nullptr;
};
