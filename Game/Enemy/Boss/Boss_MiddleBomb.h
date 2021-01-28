#pragma once

#include "Enemy/IEnemy.h"
/// <summary>
/// 
/// </summary>
class Boss_MiddleBomb final :public IEnemy
{
public:
	Boss_MiddleBomb();
	virtual ~Boss_MiddleBomb();

	void Init() override;
	void InitState() override;
	void InitIK() override;
	void Execute() override;
	void Terminate() override;
	Player::EnAttackType GetDropID() override
	{
		return Player::EnAttackType::enNone;
	}

	void PlayAnimation(const int animNo, const float interpolate = 0.2f)
	{
		m_model->Play(animNo, interpolate);
	}
	
	enum class EnStateEX :int
	{
		Luanch = TO_INT(IEnemy::EnState::enAttackA),
		Corocoro,
		FullFrontal = TO_INT(IEnemy::EnState::enNumState),
		Bash,
		Guard,
		Jump,
		Num,
	};

	enum class EnAnimEX :int
	{
		Firing = TO_INT(IEnemy::EnAnimation::enAttackA),
		FiringUp,
		FullFrontal = TO_INT(IEnemy::EnAnimation::enNumAnim),
		Bash,
		Guard,
		Jump,
		Num,
	};
private:
	void InitAnim();
private:
	//Model
	ModelRender* m_ShieldModel = nullptr;
	const float m_scale = 1.f;

	RigidBody m_rigidBody;
	MeshCollider m_meshColl;
	MeshCollider m_meshColl_ghost;
	Bone* m_shieldBone = nullptr;

	GhostObject m_shieldGhost;

	//Physics
	const float m_radius = 1.f;
	const float m_height = 3.f;
};

struct BossBombData
{
private:
	BossBombData() {};
public:
	static BossBombData& GetInstance()
	{
		static BossBombData data;
		return data;
	}

	RigidBody* rigidBody = nullptr;
	MeshCollider* meshColl = nullptr;
	GhostObject* shieldGhost = nullptr;
	bool isJumpStart = false;
	bool isBashAttack = false;
};
