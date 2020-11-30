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

		if (enemy == m_targetEnemy)
			m_targetEnemy = nullptr;

		m_enemies.erase(std::remove(m_enemies.begin(), m_enemies.end(), enemy), m_enemies.end());
		auto enemyObject = reinterpret_cast<GameObject*>(enemy);
		DeleteGO(enemyObject);
	}

	void Release();

	IEnemy* GetNearestBossEnemy() const {
		//TODO : implement this func
		if (m_enemies.size() == 0) return nullptr;

		for (int i = 0; i < m_enemies.size(); i++) {
			if (m_enemies.at(i)->IsBoss()) {
				return m_enemies.at(i);
			}
		}
		return nullptr;
		//return m_enemies[0];
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
			DebugPrint_WATA("Please set a player instance");
			assert(false);
		}
		return m_player;
	}

	const IEnemy* GetTargettingEnemy() {
		return m_targetEnemy;
	}

	void SetTargetEnemy(IEnemy* targetEnemy) {
		m_targetEnemy = targetEnemy;
	}

private:

	static const int m_numNormalEnemy = 2;

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
	const float m_defaultHeight = 130.f;
	std::vector<IEnemy*> m_enemies;
	Player* m_player = nullptr;
	IEnemy* m_targetEnemy = nullptr;

	const float BOSS_A_HP = 500.f;
	const float BOSS_A_STAMINA = 100.f;
};

