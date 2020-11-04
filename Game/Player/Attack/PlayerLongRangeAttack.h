#pragma once
class Player;
class Projectile;

class PlayerLongRangeAttack
{
public:
	PlayerLongRangeAttack();
	~PlayerLongRangeAttack();

	void Execute(Player* p);

private:
	const float m_interval = 0.2f;
	float m_intervalTimer = m_interval;

	std::vector<Projectile> m_projectiles;
};

