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
	auto center = GameManager::GetInstance().m_player->GetPosition();

	tar.y += 20.f;

	auto vecTargetToCenter = center - tar;
	auto camBeginPos = center + vecTargetToCenter;
	camBeginPos.y += 10.f;
	Quaternion rot = Quaternion::Identity;
	rot.SetRotationDegY(10.f);
	auto camEndPos = enemy->GetPosition();
	camEndPos.y += 15.f;
	camEndPos.z += 10.f;
	//rot.Apply(camEndPos);
	auto sec = 2.5f;

	//cam->Perform(
	//camBeginPos, camEndPos,
	//	tar, tar,
	//	center, sec
	//);

	cam->Perform(
		camBeginPos, camEndPos,
		tar, tar, sec
	);
	m_isBossCamPerform = true;
	m_boss = enemy;
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

	if (m_boss != nullptr && !m_isBossCamPerform)
	{
		if (m_timer > 1.7f)
		{
			auto cam = GameManager::GetInstance().m_camera;
			cam->ChangePlayerCam();
			GameManager::GetInstance().m_menu->ResumeGame();
			m_timer = 0.f;
			m_boss = nullptr;
			return;
		}
		if (m_timer > 1.f)
		{
			m_boss->Performance();
		}

		m_timer += gameTime()->GetDeltaTime();
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
