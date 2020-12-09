#pragma once
#include "IEnemyState.h"

class EnemyShortRangeComingState final : public IEnemyState
{
public:
	EnemyShortRangeComingState();
	~EnemyShortRangeComingState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
private:
	/// <summary>
	/// 移動。
	/// </summary>
	void Move(IEnemy* e);
	/// <summary>
	/// アニメーション。
	/// </summary>
	/// <returns>アニメーションが終わったかどうかを返す。</returns>
	bool Animation();
private:
	float m_danceTimer = 0.f;	//ダンス行動をするかどうかの判定を行うまでの時間計測。

	IK* m_headIK = nullptr;
};
