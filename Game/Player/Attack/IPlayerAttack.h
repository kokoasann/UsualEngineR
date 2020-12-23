#pragma once
class Player;
class IEnemy;

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

protected:
	const float m_DEFAULT_ATTACK_DAMAGE = 10.f;
	std::map<IEnemy*, bool> m_attackedEnemyMap;
};

