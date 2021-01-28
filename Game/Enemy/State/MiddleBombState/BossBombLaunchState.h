#pragma once
#include "Enemy/State/IEnemyState.h"

class MuzzleFlash;
class BossBombLaunchState final : public IEnemyState
{
public:
	BossBombLaunchState();
	~BossBombLaunchState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
	void  OnAttacked(IEnemy* e);
private:
	Bone* m_cannonBone = nullptr;
	Quaternion m_firstRot;
	float m_timer = 0.f;
	float m_timeLimit = 0.2f;
	float m_timeAnimEnd = 0.f;
	float m_timeEnd = 0.2f;
	bool m_isLaunch = false;

	MuzzleFlash* m_effect;


	const float m_damage;
};
