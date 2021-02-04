#include "stdafx.h"
#include "EnemyManager.h"
#include "Enemy/EnemyTest.h"
#include "Enemy/Boss/BossA.h"
#include "Enemy/Boss/Boss_FatMan.h"
#include "Enemy/Boss/Boss_MiddleBomb.h"
#include "EnemyTest.h"
#include <random>

#include "Zako/Zako_ShortRangeMonster.h"
#include "Zako/Zako_LongDistanceMachine.h"
#include "Zako/Zako_ExplosionMonster.h"
#include "Zako/Zako_LongBig.h"
#include "Zako/Zako_ShortBig.h"

#include "Equipment/Enemy_Bullet.h"

#include "level/Level.h"

void EnemyManager::SpawnEnemies() {
	IEnemy::StAbility ab;
	//Boss
	/*
	auto bene = NewGO<BossA>(0);
	ab.InitHP(BOSS_A_HP);
	ab.InitStamina(BOSS_A_STAMINA);
	bene->SetAbility(ab);
	Vector3 pos;
	pos.y = 100;
	bene->SetPosition(pos);
	m_enemies.push_back(bene);
	*/


	//auto e = NewGO<Zako_LongDistanceMachine>(0);
	////e->Init();
	//ab.InitHP(50000);
	//e->SetAbility(ab);
	//e->SetPosition({ -50,10,-50 });
	//m_enemies.push_back(e);

	/*ab.InitHP(BOSS_A_HP);
	ab.InitStamina(BOSS_A_STAMINA);
	Boss_MiddleBomb* boss = NewGO<Boss_MiddleBomb>(0);
	boss->SetAbility(ab);
	boss->SetPosition({ -300,50,50 });
	m_enemies.push_back(boss);*/

	static bool isCreatedEnemyBulletManager = false;
	if (!isCreatedEnemyBulletManager)
	{
		EnemyBulletManager* ebm = NewGO<EnemyBulletManager>(0);
		ebm->Allocate(400);
		isCreatedEnemyBulletManager = true;
	}
	//AllocateGO<Enemy_Bullet>(70, 0);
	Level level;
	level.Init("Assets/level/map_enemy_level.tkl", [&](LevelObjectData& objData)->bool
		{
			if (objData.name == L"LongDistanceMachine")
			{
				auto e = NewGO<Zako_LongDistanceMachine>(0);
				ab.InitHP(60);
				e->SetAbility(ab);
				e->SetPosition(objData.position * 100.f);
				e->SetRotation(objData.rotation);
				m_enemies.push_back(e);
			}
			else if (objData.name == L"ShortRangeMonster")
			{
				auto e = NewGO<Zako_ShortRangeMonster>(0);
				ab.InitHP(60);
				e->SetAbility(ab);
				e->SetPosition(objData.position * 100.f);
				e->SetRotation(objData.rotation);
				m_enemies.push_back(e);
			}
			else if (objData.name == L"BigShort")
			{
				auto e = NewGO<Zako_ShortBig>(0);
				ab.InitHP(60);
				e->SetAbility(ab);
				e->SetPosition(objData.position * 100.f);
				e->SetRotation(objData.rotation);
				m_enemies.push_back(e);
			}
			else if (objData.name == L"MeleeBoss") {
				auto bene = NewGO<BossA>(0);
				ab.InitHP(BOSS_A_HP);
				ab.InitStamina(BOSS_A_STAMINA);
				bene->SetAbility(ab);
				bene->SetPosition(objData.position * 100.f);
				bene->SetRotation(objData.rotation);
				m_enemies.push_back(bene);
			}
			else if (objData.name == L"LongFatBoss")
			{
				auto bene = NewGO<Boss_Fatman>(0);
				ab.InitHP(BOSS_A_HP);
				ab.InitStamina(BOSS_A_STAMINA);
				bene->SetAbility(ab);
				bene->SetPosition(objData.position * 100.f);
				bene->SetRotation(objData.rotation);
				m_enemies.push_back(bene);
			}
			else if (objData.name == L"BombBoss")
			{
				auto bene = NewGO<Boss_MiddleBomb>(0);
				ab.InitHP(1);//‚È‚¨‚·I
				ab.InitStamina(BOSS_A_STAMINA);
				bene->SetAbility(ab);
				bene->SetPosition(objData.position * 100.f);
				bene->SetRotation(objData.rotation);
				m_enemies.push_back(bene);
			}
			return true;
		}
	);
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
