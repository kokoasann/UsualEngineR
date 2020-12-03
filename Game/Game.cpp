#include "stdafx.h"
#include "Game.h"
#include "Level/GameStage.h"
#include "Player/Player.h"
#include "Camera/GameCamera.h"
#include "Enemy/EnemyManager.h"
#include "Enemy/IEnemy.h"
#include "GameHUD.h"
#include "GameManager.h"
#include "GameSceneMenu.h"

void Game::Release()
{
	GameManager::GetInstance().Release();
}

void Game::OnDestroy()
{
}

void Game::Awake()
{
}


void Game::OnEnterBattle(IEnemy* enemy) {

	DebugPrint_WATA("enter battle\n");

	auto cam = GameManager::GetInstance().m_camera;
	auto tar = EnemyManager::GetEnemyManager().GetNearestBossEnemy()->GetPosition();

	cam->Perform(
		cam->m_playerCameraPos, cam->m_enemyCameraPos,
		tar, tar,
		cam->m_charaPos, 0.25f
	);

}

bool Game::Start()
{
	GameManager::GetInstance().InitGameWorld();
	GameManager::GetInstance().m_gameScene = this;

	return true;
}

void Game::PreUpdate()
{

}

void Game::Update()
{
	if (g_pad[0]->IsTrigger(EnButton::enButtonStart)) {
		if (GameManager::GetInstance().m_menu->IsGamePaused()) {
			GameManager::GetInstance().m_menu->ResumeGame();
		}
		else {
			GameManager::GetInstance().m_menu->PauseGame();
		}
	}

	if (GameManager::GetInstance().m_stage->HasMapLoadingDone() and m_isCreateEnemyManager == false) {
		auto& eM = EnemyManager::GetEnemyManager();
		eM.SpawnEnemies();
		eM.SetPlayer(GameManager::GetInstance().m_player);
		m_isCreateEnemyManager = true;
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
