#pragma once
class IEnemy;

class EnemyManager
{
public:
	static EnemyManager& GetEnemyManager(){
		static EnemyManager enemyManager;
		return enemyManager;
	}

	void SpawnEnemies();

	void ApplyAoeDamage(const Vector3& attackOriginPos, float range, float damageAmount);

	const std::vector<IEnemy*> GetEnemies() const{
		return m_enemies;
	}

	const IEnemy* GetNearestEnemy() const {
		//TODO : implement this func
		return m_enemies[0];
	}

private:
	std::vector<IEnemy*> m_enemies;

};

