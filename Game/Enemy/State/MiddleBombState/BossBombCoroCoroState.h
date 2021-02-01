#pragma once
#include "Enemy/State/IEnemyState.h"

class MuzzleFlash;
class BossBombCoroCoroState final : public IEnemyState
{
public:
	BossBombCoroCoroState();
	~BossBombCoroCoroState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
	void  OnAttacked(IEnemy* e);
private:
	float m_timer = 0.f;
	const float m_timeLimit = 0.5f;
	float m_timeAnimEnd = 0.f;
	const float m_timeEnd = 0.2f;
	const float m_timeLaunchSpan = 0.5f;
	float m_timeLaunch = 0.f;
	bool m_isLaunchSleep = false;

	Bone* m_cannonBone = nullptr;
	Quaternion m_firstRot;
	MuzzleFlash* m_effect;

	bool m_isLaunch = false;
	int m_launceNum = 0;
	Vector3 m_oldIKpos = Vector3::Zero;
};
