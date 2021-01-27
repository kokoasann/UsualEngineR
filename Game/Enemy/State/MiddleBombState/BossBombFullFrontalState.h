#pragma once
#include "Enemy/State/IEnemyState.h"


class MuzzleFlash;
class BossBombFullFrontalState final : public IEnemyState
{
public:
	BossBombFullFrontalState();
	~BossBombFullFrontalState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
	void  OnAttacked(IEnemy* e);
private:
	Bone* m_cannonBone = nullptr;
	Quaternion m_firstRot;
	float m_timer = 0.f;
	float m_timeLimit = 0.2f;
	MuzzleFlash* m_effect;

	const float m_damage;
};
