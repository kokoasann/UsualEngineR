#pragma once

#include "Enemy/IEnemy.h"
#include "physics/BoxCollider.h"
/// <summary>
/// 
/// </summary>
class Zako_LongDistanceMachine final:public IEnemy
{
public:
	
	Zako_LongDistanceMachine();
	virtual ~Zako_LongDistanceMachine();

	void Init() override;
	void InitState() override;
	void InitIK() override;
	void Execute() override;
	void Terminate() override;
	Player::EnAttackType GetDropID() override 
	{ 
		return Player::EnAttackType::enNone;
	}

	void PlayAnimation(const int anim, const float interpolate = 2.f) override
	{
		m_model->Play(anim, interpolate);
	}


	using StateEnumType = UINT;
	enum class EStateEX : StateEnumType
	{
		LongDistanceTargeting = TO_INT(IEnemy::EnState::enBattleState),
		Num,
	};

	enum class EAnim : StateEnumType
	{
		Fire = TO_INT(IEnemy::EnAnimation::enAttackA),
		Num,
	};

	enum class EIK :StateEnumType
	{
		Foot1 = TO_INT(EnIK::enNumIK),
		Foot2,
		Foot3,
		Foot4,

		Num
	};

	RigidBody& GetRigidBody()
	{
		return m_rigidBody;
	}

	void SetKinematicFrag(bool b)
	{
		m_isKinematic = b;
	}

	btDefaultMotionState& GetMotionState()
	{
		return m_motionState;
	}
private:
	btDefaultMotionState m_motionState;
	//Model
	//ModelRender* m_model = nullptr;
	/*CAnimationClip m_animClip[1];
	std::vector<CAnimationClipPtr> m_animlist;*/
	float m_scale = 2.f;

	//Physics
	const float m_radius = 2.5f;
	const float m_height = 3.f;

	Quaternion m_rot = Quaternion::Identity;

	float m_bulletSpeed = 2000.f;

	BoxCollider m_box;
	RigidBody m_rigidBody;

	bool m_isKinematic = false;

	//const static StateEnumType STATE_NUM = TO_UINT(EStateEX::Num);
	//std::array<IEnemyState*, STATE_NUM> m_stateListEX;
};
