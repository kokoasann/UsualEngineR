#pragma once
#include "Enemy/State/IEnemyState.h"

class EnemyShortBigBlown final : public IEnemyState
{
public:
	EnemyShortBigBlown();
	~EnemyShortBigBlown();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
	void  OnAttacked(IEnemy* e);
private:
	IK* m_ikList[6] = { nullptr };

	Vector3 m_velocityXZ = Vector3::Zero;
	float m_velocityY = 0.f;
	bool m_isNoBlown = false;

	float m_grav = 150.f;

	float m_timer = 0.;
	float m_timeLimit = 10.f;

	//ƒfƒJ‚¢“G‚ÌUŒ‚‚È‚Ì‚ÅA‚«”ò‚Î‚µ—Ê‚É•â³‚ğ‚©‚¯‚é
	float m_bigPush = 3.0f;

	bool m_isTakeOff = false;
	bool m_isTakeOn = false;
};
