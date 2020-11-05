#pragma once
#include "../IEnemy.h"
#include "../../Player/Player.h"

/// <summary>
/// 
/// </summary>
class BossA :public IEnemy
{
public:
	void Init() override;
	void Execute() override;
	void Terminate() override;

	Player::EnAttackType GetDropID() override{
		return Player::EnAttackType::enA;
	}

private:
	//Model
	ModelRender* m_model = nullptr;

	//Physics
	const float m_radius = 1.f;
	const float m_height = 3.f;
};
