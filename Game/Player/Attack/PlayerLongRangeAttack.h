#pragma once
class Player;
class Projectile;
class MuzzleFlash;

class PlayerLongRangeAttack
{
public:
	PlayerLongRangeAttack();
	~PlayerLongRangeAttack();

	void Execute(Player* p);

private:
	const float m_interval = 0.05f;
	float m_shotIntervalTimer = m_interval;

	std::vector<Projectile> m_projectiles;

	enum class EnMuzzles {
		UpperRight,
		UpperLeft,
		LowerRight,
		LowerLeft,
		NumMuzzles
	};

	std::vector< MuzzleFlash*> m_muzzleFlashes;
};

