#pragma once
#include "Enemy/State/IEnemyState.h"
#include "Player/Player.h"
class EnemyShortBigStrongPunch final : public IEnemyState
{
public:
	EnemyShortBigStrongPunch();
	~EnemyShortBigStrongPunch();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
	void  OnAttacked(IEnemy* e);
private:
	//ƒvƒŒƒCƒ„[‚Ì‘Ì—Í‚ğ5”­‚Å0‚É‚·‚é
	const float		m_damage = Player::GetMaxHP() / 5.0f;
};
