#pragma once
#include "IEnemyState.h"

class EnemyExplosionKamikazeState final : public IEnemyState
{
public:
	EnemyExplosionKamikazeState();
	~EnemyExplosionKamikazeState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
private:
	/// <summary>
	/// �ړ��֐��B
	/// </summary>
	void Move(IEnemy* e);
private:
	float			m_bombTimer = 0;						//�����܂ł̎��Ԍv���B
};
