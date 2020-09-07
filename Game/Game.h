#pragma once

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
private:

	ModelRender* m_model;
	CAnimationClip m_animClip[1];
	std::vector<CAnimationClipPtr> m_animlist;
};