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

void GameManager::InitGameWorld() {

	m_player = NewGO<Player>(0, "Player");
	m_camera = NewGO<GameCamera>(0);
	m_stage = NewGO<GameStage>(0);
	m_gameHUD = NewGO<GameHUD>(0);
	m_itemManager = NewGO<ItemManager>(0);
	m_menu = NewGO<GameSceneMenu>(0);
	m_church = NewGO<Church>(0);

	//m_goal = NewGO<Goal>(0);
	//m_gameHUD->SetPlayer(m_player);
	//m_camera->SetPlayer(m_player);
	//m_player->SetGameCamera(m_camera);
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
}