#include "stdafx.h"
#include "GameDebugger.h"
#include "GameManager.h"
#include "Enemy/EnemyManager.h"
#include "Player/Player.h"

#include "Enemy/Boss/BossA.h"
#include "Enemy/Boss/Boss_FatMan.h"
#include "Enemy/Zako/Zako_ShortRangeMonster.h"
#include "Enemy/Zako/Zako_LongDistanceMachine.h"
#include "Enemy/Zako/Zako_ExplosionMonster.h"
#include "Enemy/Zako/Zako_LongBig.h"
#include "Enemy/Zako/Zako_ShortBig.h"
#include "Game.h"


GameDebugger::GameDebugger()
{

}

GameDebugger::~GameDebugger()
{

}



void GameDebugger::Release()
{

}

void GameDebugger::OnDestroy()
{
	Release();
}


void GameDebugger::Awake()
{

}

bool GameDebugger::Start()
{
	DebugSwitchAddCheck(DebugSwitchNewSwitch('X', 0, [&]()
		{
			if (!GameManager::GetInstance().GetPlayer())
				return;
			GameManager::GetInstance().GetPlayer()->Heal(100);
		}));
	DebugSwitchAddCheck(DebugSwitchNewSwitch('X', VK_SHIFT, [&]()
		{
			if (!GameManager::GetInstance().GetPlayer())
				return;
			GameManager::GetInstance().GetPlayer()->ChargeBoost(100);
		}));
	DebugSwitchAddCheck(DebugSwitchNewSwitch('X', VK_CONTROL, [&]()
		{
			if (!GameManager::GetInstance().GetPlayer())
				return;
			GameManager::GetInstance().GetPlayer()->ChargeEndurance(100);
		}));

	DebugSwitchAddButton(DebugSwitchNewSwitch_Button(VK_NUMPAD0, 0, [&]()
		{
			if (!GameManager::GetInstance().GetPlayer())
				return;

			auto e = NewGO<Zako_ShortRangeMonster>(0);
			IEnemy::StAbility ab;
			ab.InitStamina(100);
			ab.InitHP(60);
			e->SetAbility(ab);
			e->SetPosition(GameManager::GetInstance().GetPlayer()->GetPosition());
			EnemyManager::GetEnemyManager().AddEnemy(e);
			
		}));
	DebugSwitchAddButton(DebugSwitchNewSwitch_Button(VK_NUMPAD1, 0, [&]()
		{
			if (!GameManager::GetInstance().GetPlayer())
				return;

			auto e = NewGO<Zako_LongDistanceMachine>(0);
			IEnemy::StAbility ab;
			ab.InitStamina(100);
			ab.InitHP(60);
			e->SetAbility(ab);
			e->SetPosition(GameManager::GetInstance().GetPlayer()->GetPosition());
			EnemyManager::GetEnemyManager().AddEnemy(e);
		}));
	DebugSwitchAddButton(DebugSwitchNewSwitch_Button(VK_NUMPAD2, 0, [&]()
		{
			if (!GameManager::GetInstance().GetPlayer())
				return;

			auto e = NewGO<Zako_ShortBig>(0);
			IEnemy::StAbility ab;
			ab.InitStamina(100);
			ab.InitHP(120);
			e->SetAbility(ab);
			e->SetPosition(GameManager::GetInstance().GetPlayer()->GetPosition());
			EnemyManager::GetEnemyManager().AddEnemy(e);
		}));
	DebugSwitchAddButton(DebugSwitchNewSwitch_Button(VK_NUMPAD3, 0, [&]()
		{
			if (!GameManager::GetInstance().GetPlayer())
				return;

			auto e = NewGO<Zako_LongBig>(0);
			IEnemy::StAbility ab;
			ab.InitStamina(100);
			ab.InitHP(60);
			e->SetAbility(ab);
			e->SetPosition(GameManager::GetInstance().GetPlayer()->GetPosition());
			EnemyManager::GetEnemyManager().AddEnemy(e);
		}));

	DebugSwitchAddButton(DebugSwitchNewSwitch_Button('Q', 0, [&]()
		{
			GameManager::GetInstance().m_gameScene->GoalGatePerformance();
		}));
	return true;
}


void GameDebugger::PreUpdate()
{

}

void GameDebugger::Update()
{

}

void GameDebugger::PostUpdate()
{

}



