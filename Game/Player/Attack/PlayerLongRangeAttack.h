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
	void UpdateEffectPos();

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

	void CalcEffectsTransform();

	struct EffectsTransform {
		Vector3 m_posList[4];
		Quaternion m_rotList[4];
	};

	EffectsTransform m_effectTransforms;

	std::vector< MuzzleFlash*> m_muzzleFlashes;
};

