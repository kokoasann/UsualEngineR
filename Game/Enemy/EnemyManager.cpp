#include "stdafx.h"
#include "EnemyManager.h"
#include "Enemy/EnemyTest.h"
#include "Enemy/Boss/BossA.h"
#include "EnemyTest.h"
#include <random>

#include "Zako/Zako_LongDistanceMachine.h"

void EnemyManager::SpawnEnemies() {
	IEnemy::StAbility ab;
	//Boss
	auto bene = NewGO<BossA>(0);

	ab.InitHP(BOSS_A_HP);
	bene->SetAbility(ab);
	Vector3 pos;
	pos.y = 100;
	bene->SetPosition(pos);

	m_enemies.push_back(bene);

	auto e = NewGO<Zako_LongDistanceMachine>(0);
	//e->Init();
	e->SetAbility(ab);
	e->SetPosition({ -50,10,-50 });
	m_enemies.push_back(e);

	return;
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
			m_enemies[i]->ApplyDamage(damageAmount);
#ifdef _PRINT_ENEMY_DAMAGE
			std::string s = "Apply AoE Damage!Amount :";
			s += damageAmount;
			DebugPrint_WATA(s.c_str());
#endif // _PRINT_ENEMY_DAMAGE
		}
	}
}
