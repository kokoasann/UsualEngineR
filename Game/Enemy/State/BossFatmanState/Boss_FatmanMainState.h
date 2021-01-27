#pragma once
#include "Enemy/State/IEnemyState.h"

class Boss_FatmanAngryPerform;


class Boss_FatmanMainState final : public IEnemyState
{
public:
	Boss_FatmanMainState();
	~Boss_FatmanMainState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
private:
	/// <summary>
	/// �������U���̃X�e�[�g�J�ځB
	/// </summary>
	IEnemyState* LongRangeAttack(IEnemy* e);
	/// <summary>
	/// �o�g���t�F�[�Y�̑J�ځB
	/// </summary>
	void ChangeBattleState(IEnemy* e);
private:
	float m_fatTimer = 0.f;

	//���o�֌W�B
	bool m_isPerformed = false;
	bool m_isAngryPerformed = false;
	bool m_isBmp = false;
	Boss_FatmanAngryPerform* m_bap = nullptr;

	//IK�֌W�B
	const int IK_NUM = 2;
	IK* m_BeamIk[2];					//IK�B���E��IK�̏�񂪊i�[����Ă���B
	IK* m_ShootIk[2];					//IK�B���E��IK�̏�񂪊i�[����Ă���B
};
