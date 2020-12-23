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
	//�v���C���[�̗̑͂�5����0�ɂ���
	const float		m_damage = Player::GetMaxHP() / 5.0f;
};
