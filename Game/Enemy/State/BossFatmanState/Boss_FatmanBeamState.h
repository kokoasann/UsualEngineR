#pragma once
#include "Enemy/State/IEnemyState.h"
#include "Player/Player.h"

class Boss_FatmanBeamState final : public IEnemyState
{
public:
	Boss_FatmanBeamState();
	~Boss_FatmanBeamState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
private:
	bool Judge(IEnemy* e);
private:
	Vector3			m_position = Vector3::Zero;
	const float		m_damage = Player::GetMaxHP() / 2000.0f;
	float			m_damageTimer = 0;
};