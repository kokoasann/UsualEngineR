#include "stdafx.h"
#include "Game.h"
#include "Level/GameStage.h"
#include "Player/Player.h"
#include "Enemy/EnemyTest.h"
#include "Enemy/Boss/BossA.h"
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

	auto ene = NewGO<BossA>(0);
	m_enemies.push_back(ene);


	m_camera->SetCharaPos(m_player->GetPosition());
	const auto& epos = m_enemies[0]->GetPosition();
	m_camera->SetTarget(epos);

	return true;
}

void Game::PreUpdate()
{

}

void Game::Update()
{
	m_camera->SetCharaPos(m_player->GetPosition());
	const auto& epos = m_enemies[0]->GetPosition();
	m_camera->SetTarget(epos);
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
