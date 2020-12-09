#pragma once
#include "IEnemyState.h"

class EnemyShortRangeComingState final : public IEnemyState
{
public:
	EnemyShortRangeComingState();
	~EnemyShortRangeComingState();
	void Enter(IEnemy* e) override;
	IEnemyState* Update(IEnemy* e) override;
	void Exit(IEnemy* e) override;
private:
	/// <summary>
	/// �ړ��B
	/// </summary>
	void Move(IEnemy* e);
	/// <summary>
	/// �A�j���[�V�����B
	/// </summary>
	/// <returns>�A�j���[�V�������I��������ǂ�����Ԃ��B</returns>
	bool Animation();
private:
	float m_danceTimer = 0.f;	//�_���X�s�������邩�ǂ����̔�����s���܂ł̎��Ԍv���B

	IK* m_headIK = nullptr;
};
