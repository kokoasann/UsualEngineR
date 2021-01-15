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

	
	enum class EnStateEX :int
	{
		Expand = TO_INT(IEnemy::EnState::enNumState),
		Num,
	};

	enum class EnAnimEX :int
	{
		Expand = TO_INT(IEnemy::EnAnimation::enNumAnim),
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

	Bone* m_shieldBone = nullptr;

	//Physics
	const float m_radius = 1.f;
	const float m_height = 3.f;
};
