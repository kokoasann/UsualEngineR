#pragma once
#include "../Enemy/IEnemy.h"
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

	void DestroyEnemy(IEnemy* enemy) {
		m_enemies.erase(std::remove(m_enemies.begin(), m_enemies.end(), enemy), m_enemies.end());
		auto enemyObject = reinterpret_cast<GameObject*>(enemy);
		DeleteGO(enemyObject);
	}

	void Release();

	IEnemy* GetNearestBossEnemy() const {
		//TODO : implement this func
		if (m_enemies.size() == 0) return nullptr;
		return m_enemies[0];
	}

	IEnemy* GetNearestEnemy() const {
		//TODO : implement this func
		if (m_enemies.size() == 0) return nullptr;
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

	static const int m_numNormalEnemy = 10;

	//SoA 
	/*
	struct StEnemyTable {
		Vector3 pos[m_numNormalEnemy];
		float health[m_numNormalEnemy];
	};
	*/

	const float m_normalEnemyHealth = 500.f;
	const float m_minSpawnRange = -100.f;
	const float m_maxSpawnRange = 100.f;
	const float m_defaultHeight = 150.f;
	std::vector<IEnemy*> m_enemies;
	Player* m_player = nullptr;
};

