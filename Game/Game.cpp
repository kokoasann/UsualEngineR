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

	m_camera->SetCharaPos(m_player->GetPosition());

	auto& eM = EnemyManager::GetEnemyManager();
	eM.SpawnEnemies();
	eM.SetPlayer(m_player);

	auto enemy = eM.GetNearestEnemy();
	m_camera->SetTarget(enemy->GetPosition());

	return true;
}

void Game::PreUpdate()
{

}

void Game::Update()
{
	m_camera->SetCharaPos(m_player->GetPosition());
	auto enemy = EnemyManager::GetEnemyManager().GetNearestEnemy();
	m_camera->SetTarget(enemy->GetPosition());
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
