#pragma once
class IEnemy;
class Player;

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

	void SetPlayer(Player* player) {
		m_player = player;
	}

	Player* GetPlayer() const{
		if (m_player == nullptr) {
			printf("Please set a player instance");
			assert(false);
		}
		return m_player;
	}

private:
	std::vector<IEnemy*> m_enemies;
	Player* m_player = nullptr;
};

