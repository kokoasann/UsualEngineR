#pragma once

#include "Enemy/IEnemy.h"
/// <summary>
/// 
/// </summary>
class XXX final :public IEnemy
{
public:
	XXX();
	virtual ~XXX();

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
	float m_scale = 1.f;

	//Physics
	const float m_radius = 1.f;
	const float m_height = 3.f;
};