#pragma once
class Player;
class IPlayerAttack
{
public :
	IPlayerAttack() {};
	virtual ~IPlayerAttack() {};
	virtual void Init(Player* player, int combo) = 0;
	virtual void Execute(Player* player) = 0;
	virtual const bool IsDone() = 0;
	virtual const float GetInterval() = 0;
	virtual const bool IsContinueAttack() = 0;
};

