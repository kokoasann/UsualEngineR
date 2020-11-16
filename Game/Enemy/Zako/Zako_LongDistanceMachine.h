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
	void Execute() override;
	void Terminate() override;
	Player::EnAttackType GetDropID() override 
	{ 
		return Player::EnAttackType::enNone;
	}

private:
	//Model
	ModelRender* m_model = nullptr;
	CAnimationClip m_animClip[1];
	std::vector<CAnimationClipPtr> m_animlist;
	Vector3 m_scale = { 5.f,5.f,5.f };

	//Physics
	const float m_radius = 1.f;
	const float m_height = 3.f;
};
