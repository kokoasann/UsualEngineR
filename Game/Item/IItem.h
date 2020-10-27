#pragma once
class Player;

class IItem
{
public:
	IItem();
	virtual ~IItem();
	virtual void Apply(Player* player) = 0;
};

