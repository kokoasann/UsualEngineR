#pragma once
class IPlayerAttack
{
public :
	IPlayerAttack() {};
	virtual ~IPlayerAttack() {};
	virtual void Init(int combo) = 0;
	virtual void Execute() = 0;
	virtual const bool IsDone() = 0;
	virtual const float GetInterval() = 0;
	virtual const bool IsContinueAttack() = 0;
};

