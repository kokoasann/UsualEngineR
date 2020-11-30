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

void GameManager::InitGameWorld() {

	m_player = NewGO<Player>(0, "Player");
	m_camera = NewGO<GameCamera>(0);
	m_stage = NewGO<GameStage>(0);
	m_gameHUD = NewGO<GameHUD>(0);
	m_itemManager = NewGO<ItemManager>(0);
	m_menu = NewGO<GameSceneMenu>(0);

	m_gameHUD->SetPlayer(m_player);
	m_camera->SetPlayer(m_player);
	//m_player->SetGameCamera(m_camera);

	auto& eM = EnemyManager::GetEnemyManager();
	eM.SpawnEnemies();
	eM.SetPlayer(m_player);

	auto enemy = eM.GetNearestEnemy();
}

void GameManager::Release() {
	DeleteGO(m_player);
	DeleteGO(m_camera);
	DeleteGO(m_stage);
	DeleteGO(m_gameHUD);
	DeleteGO(m_itemManager);
	DeleteGO(m_menu);
}