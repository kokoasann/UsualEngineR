#include "stdafx.h"
#include "GameManager.h"
#include "Level/GameStage.h"
#include "Player/Player.h"
#include "Camera/GameCamera.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/IEnemy.h"
#include "GameHUD.h"
#include "GameManager.h"
#include "Item/ItemManager.h"
#include "GameSceneMenu.h"
#include "Goal.h"
#include "Church.h"
#include "Fade.h"
#include "Player/Pod/Pod.h"

void GameManager::InitGameWorld() {

	//m_player = NewGO<Player>(0, "Player");
	m_camera = NewGO<GameCamera>(0);
	m_stage = NewGO<GameStage>(0);
	m_gameHUD = NewGO<GameHUD>(1);
	m_itemManager = NewGO<ItemManager>(0);
	m_menu = NewGO<GameSceneMenu>(2);
	m_church = NewGO<Church>(0);
	//m_gameHUD->SetPlayer(m_player);
	//m_camera->SetPlayer(m_player);
	//m_player->SetGameCamera(m_camera);
	m_nvm.Init();
}

void GameManager::Release() {
	DeleteGO(m_player);
	m_player = nullptr;
	DeleteGO(m_camera);
	m_camera = nullptr;
	DeleteGO(m_stage);
	m_stage = nullptr;
	DeleteGO(m_gameHUD);
	m_gameHUD = nullptr;
	DeleteGO(m_itemManager);
	m_itemManager = nullptr;
	DeleteGO(m_menu);
	m_menu = nullptr;
	DeleteGO(m_goal);
	m_goal = nullptr;
	DeleteGO(m_church);
	m_church = nullptr;
	DeleteGO(m_pod);
	m_pod = nullptr;
}

void GameManager::SpawnPlayer() {
	if(m_player == nullptr)
		m_player = NewGO<Player>(0);
	if(m_pod ==nullptr)
		m_pod = NewGO<Pod>(0);
}

void GameManager::AppearGoal() {
	m_goal = NewGO<Goal>(0);
}