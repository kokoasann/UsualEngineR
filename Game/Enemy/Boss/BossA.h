#pragma once
#include "../IEnemy.h"

/// <summary>
/// 
/// </summary>
class BossA :public IEnemy
{
public:
	void Init() override;
	void Execute() override;
	void Terminate() override;
private:
	//Model
	ModelRender* m_model = nullptr;

	//Physics
	const float m_radius = 1.f;
	const float m_height = 3.f;
};
