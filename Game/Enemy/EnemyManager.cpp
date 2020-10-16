#include "stdafx.h"
#include "EnemyManager.h"
#include "Enemy/EnemyTest.h"
#include "Enemy/Boss/BossA.h"
#include "EnemyTest.h"

void EnemyManager::SpawnEnemies() {
	auto bene = NewGO<BossA>(0);
	m_enemies.push_back(bene);

	auto nene = NewGO<EnemyTest>(0);
	m_enemies.push_back(nene);
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
