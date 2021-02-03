#pragma once
class IEnemy;

/// <summary>
/// �Q�[��
/// </summary>
class Game :public GameObject
{
public:
	/// <summary>
	/// �{�J���B�m�ۂ������̂��J�����邽�߂̊֐��B
	/// </summary>
	virtual void Release() override;

	/// <summary>
	/// ���̃I�u�W�F�N�g�̏�������Q�[���I�u�W�F�N�g�����������߂̊֐��B
	/// �ʓ|���������Release�֐��Ɠ����ł��悢�B
	/// </summary>
	virtual void OnDestroy() override;


	/// <summary>
	/// NewGO���ɑ����ɌĂ΂��֐��B
	/// �R���X�g���N�^�̑���Ɏg�����B
	/// </summary>
	void Awake() override;

	/// <summary>
	/// �X�^�[�g�֐��B
	/// </summary>
	/// <returns>true��Ԃ��Ȃ�����A������Start���Ăё�����</returns>
	bool Start() override;


	/// <summary>
	/// �X�V�B�̑O�ɌĂ΂��X�V�B
	/// </summary>
	void PreUpdate() override;
	/// <summary>
	/// �X�V�B
	/// </summary>
	void Update() override;
	/// <summary>
	/// �X�V�B�̌�ɌĂ΂��X�V�B
	/// </summary>
	void PostUpdate() override;


	/// <summary>
	/// �`��B
	/// </summary>
	void Render() override;

	/// <summary>
	/// ��O�ɕ\�������̕`��B
	/// </summary>
	void PostRender() override;

	void OnEnterBattle(IEnemy* enemy);

	void OnEnemyDied(IEnemy* enemy);

	void OnItemUnlocked();

	void EndBossPerform()
	{
		m_isBossCamPerform = false;
	}

	void OnGoal();

	void GoalGatePerformance();

	void ToTitle();

	void Restart();

private:
	bool m_isCreateEnemyManager = false;		//EnemyManager���쐬�������ǂ����B
	bool m_isBossCamPerform = false;
	IEnemy* m_boss = nullptr;
	bool m_isCalledExplode = false ;
	float m_timer = 0.f;

	bool m_isGenerateGoalAfterBossPerformance = false;
	float m_goalAppearTimer = 0.f;
	float m_goalAppearTime = 2.8f;

	double m_clearTimer = 0.f;

	bool m_isCleared = false;

	bool m_restartFlag = false;
	bool m_toTitleFlag = false;
};