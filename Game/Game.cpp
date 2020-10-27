#include "stdafx.h"
#include "Game.h"
#include "Level/GameStage.h"
#include "Player/Player.h"
#include "Camera/GameCamera.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/IEnemy.h"
#include "GameHUD.h"
#include "GameManager.h"

void Game::Release()
{
	GameManager::GetInstance().Release();
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
	GameManager::GetInstance().InitGameWorld();
	return true;
}

void Game::PreUpdate()
{

}

void Game::Update()
{
	auto enemy = EnemyManager::GetEnemyManager().GetNearestEnemy();
	if (enemy != nullptr) {
		GameManager::GetInstance().m_camera->SetEnemy(enemy);
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
