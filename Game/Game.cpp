#include "stdafx.h"
#include "Game.h"
#include "Level/GameStage.h"
#include "Player/Player.h"
#include "Camera/GameCamera.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/IEnemy.h"
#include "GameHUD.h"

void Game::Release()
{
	DeleteGO(m_player);
	DeleteGO(m_camera);
	DeleteGO(m_stage);
	DeleteGO(m_gameHUD);
}

void Game::OnDestroy()
{
}

void Game::Awake()
{
	AllocConsole();
	freopen("CON", "r", stdin);
	freopen("CON", "w", stdout);
	freopen("CON", "w", stderr);
	
}

bool Game::Start()
{
	m_player = NewGO<Player>(0, "Player");
	m_camera = NewGO<GameCamera>(0);
	m_stage = NewGO<GameStage>(0);
	m_gameHUD = NewGO<GameHUD>(0);

	m_gameHUD->SetPlayer(m_player);
	m_camera->SetPlayer(m_player);

	auto& eM = EnemyManager::GetEnemyManager();
	eM.SpawnEnemies();
	eM.SetPlayer(m_player);

	auto enemy = eM.GetNearestEnemy();

	return true;
}

void Game::PreUpdate()
{

}

void Game::Update()
{
	auto enemy = EnemyManager::GetEnemyManager().GetNearestEnemy();
	if (enemy != nullptr) {
		m_camera->SetEnemy(enemy);
	}
}

void Game::PostUpdate()
{

}

void Game::Render()
{
}

void Game::PostRender()
{
}
