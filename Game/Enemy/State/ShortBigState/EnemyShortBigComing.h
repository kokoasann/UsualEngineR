#pragma once
#include "Enemy/State/IEnemyState.h"

class EnemyShortBigComing final : public IEnemyState
{
public:
	EnemyShortBigComing();
	~EnemyShortBigComing();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
	
private:
	/// <summary>
	/// �ړ�
	/// </summary>
	/// <param name="e"></param>
	void Move(IEnemy* e);
	/// <summary>
	/// �A�j���[�V����
	/// </summary>
	/// <returns>�A�j���[�V�������I��������ǂ�����Ԃ��B</returns>
	bool IsAnimation();
private:
	float m_danceTimer = 0.f;	//�_���X�s�������邩�ǂ����̔�����s���܂ł̎��Ԍv���B

	IK* m_headIK = nullptr;
};
