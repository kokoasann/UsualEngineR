#pragma once
#include "IItem.h"

//‰ñ•œƒAƒCƒeƒ€
class RedPotion : public IItem
{
public:
	RedPotion();
	~RedPotion() override;
	virtual void Apply(Player* p) override;
private:
	const float m_healAmount = 10.f;
};

