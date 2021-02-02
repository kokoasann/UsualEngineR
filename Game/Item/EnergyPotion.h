#pragma once
#include "IItem.h"

class SmokeEffect;
//回復アイテム
class EnergyPotion : public IItem
{
public:
	EnergyPotion();
	~EnergyPotion() override;
	virtual void Apply(Player* p) override;
	virtual void Update() override;

private:
	const float m_chargeAmount = 10.f;
	const float modelpos_Y = 0.0f;

	float m_angle = 0.0f;

	float m_PI = 3.14159265;
	int m_count = 0;

	float m_deltaTimeSt = 0;
	//model
	ModelRender* m_model = nullptr;
	//effect
	SmokeEffect* m_effect = nullptr;
	Vector3 m_scale = { 0.5f,0.5,0.5f };
	Vector4 m_color = { 1,1,0,1 };
};

