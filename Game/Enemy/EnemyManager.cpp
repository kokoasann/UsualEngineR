#include "stdafx.h"
#include "EnemyManager.h"
#include "Enemy/EnemyTest.h"
#include "Enemy/Boss/BossA.h"
#include "EnemyTest.h"
#include <random>

void EnemyManager::SpawnEnemies() {
	//Boss
	auto bene = NewGO<BossA>(0);

	IEnemy::StAbility ab;
	ab.InitHP(200.f);
	bene->SetAbility(ab);
	Vector3 pos;
	pos.y = 100;
	bene->SetPosition(pos);

	m_enemies.push_back(bene);

	//Zako!
	std::random_device rd;
	std::mt19937 mt(rd()); 
	std::uniform_int_distribution<> rand100(m_minSpawnRange, m_maxSpawnRange);

	for (int i = 0; i < m_numNormalEnemy; i++) {
		auto nene = NewGO<EnemyTest>(0);
		ab.InitHP(m_normalEnemyHealth);
		nene->SetAbility(ab);

		//random spawn
		Vector3 pos;
		pos.x = rand100(mt);
		pos.z = rand100(mt);
		pos.y = m_defaultHeight;
		nene->SetPosition(pos);
		m_enemies.push_back(nene);
	}
}

void EnemyManager::Release() {
	for (int i = 0; i < m_enemies.size(); i++) {
		DeleteGO(m_enemies[i]);
	}
	m_enemies.clear();
	m_enemies.shrink_to_fit();
}

void EnemyManager::ApplyAoeDamage(const Vector3& attackOriginPos, float range, float damageAmount) {
	for (int i = 0; i < m_enemies.size(); i++) {
		const auto& pos = m_enemies[i]->GetPosition();
		auto vecEnemyToPlayer = attackOriginPos - pos;
		if (vecEnemyToPlayer.Length() < range) {
			printf("Apply AoE Damage! Amount : %f\n", damageAmount);
			m_enemies[i]->ApplyDamage(damageAmount);
		}
	}
}
