#include "stdafx.h"
#include "Game.h"
#include "Level/GameStage.h"
#include "Player/Player.h"
#include "Enemy/EnemyTest.h"
#include "Camera/GameCamera.h"

void Game::Release()
{
	DeleteGO(m_player);
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
	m_player = NewGO<Player>(0);
	m_camera = NewGO<GameCamera>(0);
	m_stage = NewGO<GameStage>(0);
	auto tarPos = m_player->GetPosition();
	m_camera->SetTarget(tarPos);

	return true;
}

void Game::PreUpdate()
{
}

void Game::Update()
{

}

void Game::PostUpdate()
{
	auto tarPos = m_player->GetPosition();
	m_camera->SetTarget(tarPos);
}

void Game::Render()
{
}

void Game::PostRender()
{
}
