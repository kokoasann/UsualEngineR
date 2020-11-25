#pragma once

#include "Enemy/IEnemy.h"
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
		Idle,
		LongDistanceTargeting = TO_INT(IEnemy::EnState::enBattleState),
		Dead,
		Num,
	};

	enum class EAnim : StateEnumType
	{
		Idle,
		Walk,
		Fire,
		Num,
	};
private:
	
	//Model
	//ModelRender* m_model = nullptr;
	CAnimationClip m_animClip[1];
	std::vector<CAnimationClipPtr> m_animlist;
	float m_scale = 2.f;

	//Physics
	const float m_radius = 1.f;
	const float m_height = 3.f;

	Quaternion m_rot = Quaternion::Identity;

	float m_bulletSpeed = 2000.f;

	//const static StateEnumType STATE_NUM = TO_UINT(EStateEX::Num);
	//std::array<IEnemyState*, STATE_NUM> m_stateListEX;
};
