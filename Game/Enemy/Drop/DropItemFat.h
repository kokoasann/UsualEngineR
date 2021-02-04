#pragma once
#include "../../Player/Player.h"

/// <summary>
/// 
/// </summary>
class SmokeEffect;
class DropItemFat :public GameObject
{
public:
	DropItemFat();
	virtual ~DropItemFat();

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

	void SetPosition(const Vector3& pos) {
		m_position = pos;
	}

private:

	//const
	const float m_GRASP_RANGE = 5.f;
	const Player::EnAttackType m_typeId = Player::EnAttackType::enRemoteAttackPreset;

	//Model
	ModelRender* m_model = nullptr;
	Vector3 m_position = { 0,15,-20 };	
	Quaternion m_rotation = Quaternion::Identity;
	const float m_scale = 1.f;

	//effect�֘A
	SmokeEffect* m_effect = nullptr;
	float m_PI = 3.14159265;
	float m_angle = 0.0f;
	float m_deltaTimeSt = 0;

};
