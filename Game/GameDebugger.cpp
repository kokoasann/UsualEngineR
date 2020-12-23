#include "stdafx.h"
#include "GameDebugger.h"
#include "GameManager.h"
#include "Player/Player.h"


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



