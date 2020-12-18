#pragma once
#include "IItem.h"

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

	//model
	ModelRender* m_model = nullptr;
	Vector3 m_scale = { 0.5f,0.5,0.5f };
	Vector4 m_color = { 1,1,0,1 };
};

