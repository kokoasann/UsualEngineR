#pragma once
#include "IItem.h"

//‰ñ•œƒAƒCƒeƒ€
class RedPotion : public IItem
{
public:
	RedPotion();
	~RedPotion() override;
	virtual void Apply(Player* p) override;
	virtual void Update() override;

private:
	const float m_healAmount = 10.f;

	//model
	ModelRender* m_model = nullptr;
	Vector3 m_scale = { 0.5f,0.5,0.5f };
	Vector4 m_color = { 1,0,0,1 };
};

