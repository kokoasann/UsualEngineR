#pragma once
#include "Enemy/State/IEnemyState.h"
#include "Player/Player.h"
class EnemyShortBigPunch final : public IEnemyState
{
public:
	EnemyShortBigPunch();
	~EnemyShortBigPunch();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
	void  OnAttacked(IEnemy* e);
private:
	//�v���C���[�̗̑͂�10����0�ɂ���B
	const float		m_damage = Player::GetMaxHP() / 10.0f;
	float m_fowardMove = 100.f;

	Vector3 m_forward = Vector3::Zero;
};
