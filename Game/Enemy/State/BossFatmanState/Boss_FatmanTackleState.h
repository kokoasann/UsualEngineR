#pragma once
#include "Enemy/State/IEnemyState.h"
#include "Player/Player.h"

class Boss_FatmanTackleState final : public IEnemyState
{
public:
	Boss_FatmanTackleState();
	~Boss_FatmanTackleState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
private:
	const float		m_damage = Player::GetMaxHP() / 10.f;
	float			m_timer = 0.f;
	Vector3			m_playerPosition = Vector3::Zero;
	bool			m_isSound = false;
};
