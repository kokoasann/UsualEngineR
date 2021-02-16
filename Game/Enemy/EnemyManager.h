#pragma once
#include "../Enemy/IEnemy.h"
class Player;
class EnemyBulletManager;

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

	IEnemy* GetNearestBossEnemy(const Vector3& pos) const {
		if (m_enemies.size() == 0) return nullptr;

		int nearestIndex = 0;
		float minLen = FLT_MAX;

		for (int i = 0; i < m_enemies.size(); i++) {

			if (m_enemies.at(i)->GetCurrentState() == m_enemies.at(i)->GetState(TO_INT(IEnemy::EnState::enDeadState))) {
				continue;
			}

			if (!m_enemies.at(i)->IsBoss()) {
				continue;
			}

			float len = (m_enemies.at(i)->GetPosition() - pos).Length();
			if (len < minLen) {
				nearestIndex = i;
				minLen = len;
			}
		}

		if (m_enemies.at(nearestIndex)->GetCurrentState() == m_enemies.at(nearestIndex)->GetState(TO_INT(IEnemy::EnState::enDeadState)))
			return nullptr;

		return m_enemies.at(nearestIndex);
	}

	IEnemy* GetNearestEnemy(const Vector3 pos) const {
		if (m_enemies.size() == 0) return nullptr;

		int nearestIndex = 0;
		float minLen = FLT_MAX;

		for (int i = 0; i < m_enemies.size(); i++) {

			if (m_enemies.at(i)->GetCurrentState() == m_enemies.at(i)->GetState(TO_INT(IEnemy::EnState::enDeadState))) {
				continue;
			}

			float len = (m_enemies.at(i)->GetPosition() - pos).Length();
			if (len < minLen) {
				nearestIndex = i;
				minLen = len;
			}
		}

		if (m_enemies.at(nearestIndex)->GetCurrentState() == m_enemies.at(nearestIndex)->GetState(TO_INT(IEnemy::EnState::enDeadState)))
			return nullptr;
			
		return m_enemies.at(nearestIndex);
	}

	IEnemy* GetTargettingEnemy() const{
		return m_targetEnemy;
	}

	void SetTargetEnemy(IEnemy* targetEnemy) {
		m_targetEnemy = targetEnemy;
	}

	void AddEnemy(IEnemy* e)
	{
		m_enemies.push_back(e);
	}

	void AddCorpseCount(const int val = 1) {
		assert(val > 0);
		m_numCorpse += val;
	}

	const int GetCorpseCount() {
		return m_numCorpse;
	}

	 void SetMeleeBoss(IEnemy* instance) {
		 m_meleeBoss = instance;
	}
	IEnemy* GetMeleeBoss() {
		return m_meleeBoss;
	}

	void SetBombBoss(IEnemy* instance) {
		m_bombBoss = instance;
	}
	IEnemy* GetBombBoss() {
		return m_bombBoss;
	}

	void SetFatBoss(IEnemy* instance) {
		m_fatBoss = instance;
	}
	IEnemy* GetFatBoss() {
		return m_fatBoss;
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
	IEnemy* m_targetEnemy = nullptr;

	IEnemy* m_meleeBoss = nullptr;
	IEnemy* m_bombBoss = nullptr;
	IEnemy* m_fatBoss = nullptr;

	const float BOSS_A_HP = 1500.f;
	const float BOSS_A_STAMINA = 100.f;

	int m_numCorpse = 0;

	//EnemyBulletManager* m_enemyBulletManager = 0;
};

