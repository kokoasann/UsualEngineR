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
#include "Title.h"

void Game::Release()
{
	GameManager::GetInstance().Release();
	EnemyManager::GetEnemyManager().Release();
}

void Game::OnDestroy()
{
	Release();
}

void Game::Awake()
{
}

void Game::OnEnterBattle(IEnemy* enemy) {

	DebugPrint_WATA("enter battle\n");

	auto cam = GameManager::GetInstance().m_camera;
	auto tar = enemy->GetPosition();
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
	auto interval = 1.7f;
	//cam->Perform(
	//camBeginPos, camEndPos,
	//	tar, tar,
	//	center, sec
	//);

	cam->Perform(
		camBeginPos, camEndPos,
		tar, tar, sec, interval
	);
	m_isBossCamPerform = true;
	m_boss = enemy;
}

void Game::OnEnemyDied(IEnemy* enemy) {

	DebugPrint_WATA("enemy died\n");

	auto cam = GameManager::GetInstance().m_camera;
	auto tar = enemy->GetPosition();
	tar.y += 20.f;

	auto eneForward = enemy->GetForward();
	auto camEndPos = enemy->GetPosition() + eneForward * 45.f;
	camEndPos.y += 15.f;
	auto sec = 1.f;
	auto interval = 1.7f;

	cam->Perform(
		camEndPos, camEndPos,
		tar, tar, sec, interval
	);

	m_isBossCamPerform = true;
	m_boss = enemy;

}

void Game::OnItemUnlocked() {
	DebugPrint_WATA("Nannka Item has been unlocked\n");
	auto player = GameManager::GetInstance().m_player;
	auto cam = GameManager::GetInstance().m_camera;
	auto tar = player->GetPosition();

	auto plForward = player->GetForward();
	auto camBeginPos = player->GetPosition() + plForward * 45.f;
	auto camEndPos = player->GetPosition() + plForward * 25.f;
	camBeginPos.y += 5.f;
	auto sec = 1.5f;
	auto interval = 0.f;

	cam->Perform(
		camBeginPos, camEndPos,
		tar, tar, sec, interval
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

	if (GameManager::GetInstance().m_stage->HasMapLoadingDone() and m_isCreateEnemyManager == false) {
		auto& eM = EnemyManager::GetEnemyManager();
		eM.SpawnEnemies();
		m_isCreateEnemyManager = true;
	}

	if (m_boss != nullptr && !m_isBossCamPerform)
	{
		if (m_timer > 1.7f)
		{
			if (m_boss->GetCurrentState() == m_boss->GetState(TO_INT(IEnemy::EnState::enDeadState))) {
				GameObject* enemy = reinterpret_cast<GameObject*>(m_boss);
				EnemyManager::GetEnemyManager().DestroyEnemy(m_boss);
			}
			//auto cam = GameManager::GetInstance().m_camera;
			//cam->ChangePlayerCam();
			//GameManager::GetInstance().m_menu->ResumeGame();
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
	if (g_pad[0]->IsTrigger(EnButton::enButtonStart)) {
		if (GameManager::GetInstance().m_menu->IsGamePaused()) {
			GameManager::GetInstance().m_menu->ResumeGame();
		}
		else {
			GameManager::GetInstance().m_menu->PauseGame();
		}
	}

	if (g_pad[0]->IsTrigger(enButtonLB3)) {
		NewGO<Title>(0);
		auto go = reinterpret_cast<GameObject*>(this);
		DeleteGO(go);
	}
}

void Game::Render()
{
}

void Game::PostRender()
{
}
