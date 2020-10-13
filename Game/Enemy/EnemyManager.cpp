#include "stdafx.h"
#include "EnemyManager.h"
#include "Enemy/EnemyTest.h"
#include "Enemy/Boss/BossA.h"

void EnemyManager::SpawnEnemies() {
	auto ene = NewGO<BossA>(0);
	m_enemies.push_back(ene);
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
