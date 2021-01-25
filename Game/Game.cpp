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
#include "Fade.h"
#include "Goal.h"
#include "Result.h"

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

void Game::OnGoal() {
	DebugPrint_WATA("goal\n");
	//NewGO<Title>(0);
	auto result = NewGO<Result>(0);
	result->Init(m_clearTimer);

	auto go = reinterpret_cast<GameObject*>(this);
	DeleteGO(go);
}

void Game::OnEnterBattle(IEnemy* enemy) {

	DebugPrint_WATA("enter battle\n");

	auto cam = GameManager::GetInstance().m_camera;
	auto tar = enemy->GetPosition();
	auto center = GameManager::GetInstance().m_player->GetPosition();

	//Rotate.
	auto vecToPlayer = GameManager::GetInstance().GetPlayer()->GetPosition() - enemy->GetPosition();
	vecToPlayer.Normalize();
	Quaternion eneRot = Quaternion::Identity;
	auto theta = atan2(vecToPlayer.x, vecToPlayer.z);
	theta = theta * (180.f / Math::PI);
	eneRot.SetRotationDegY(theta);
	enemy->SetRotation(eneRot);
	enemy->GetModel()->SetRotation(eneRot);

	tar.y += 20.f;

	auto vecTargetToCenter = center - tar;
	auto camBeginPos = center + vecTargetToCenter;
	camBeginPos.y += 30.f;
	Quaternion rot = Quaternion::Identity;
	rot.SetRotationDegY(10.f);

	auto camEndPos = enemy->GetPosition();
	Vector3 vecToCamEndPos = { 0.f,15.f,10.f };
	eneRot.Apply(vecToCamEndPos);
	camEndPos = enemy->GetPosition() + vecToCamEndPos;

	//rot.Apply(camEndPos);
	auto sec = 2.5f;
	auto interval = 1.7f;
	//cam->Perform(
	//camBeginPos, camEndPos,
	//	tar, tar,
	//	center, sec
	//);

	EnemyManager::GetEnemyManager().SetTargetEnemy(enemy);

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

	auto& eM = EnemyManager::GetEnemyManager();
	if (enemy->IsBoss()) {
		GameManager::GetInstance().AddDeadBossCount();
		//TODO : const int NUM_BOSS = 3;
		const int NUM_BOSS = 1; //debug code
		if (GameManager::GetInstance().GetDeadBossCount() == NUM_BOSS) {
			m_isGenerateGoalAfterBossPerformance = true;
		}
	}

}

void Game::OnItemUnlocked() {
	DebugPrint_WATA("Nannka Item has been unlocked\n");
	auto player = GameManager::GetInstance().m_player;
	auto cam = GameManager::GetInstance().m_camera;
	auto tar = player->GetPosition();

	auto plForward = player->GetForward();
	auto camBeginPos = player->GetPosition() + plForward * 45.f;
	auto camEndPos = player->GetPosition() + plForward * 25.f;
	camBeginPos.y += 20.f;
	camEndPos.y += 20.f;
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

	m_clearTimer  = 0.f;

	return true;
}

void Game::PreUpdate()
{
}

void Game::GoalGatePerformance() {
	DebugPrint_WATA("Gate Open!!\n");

	GameManager::GetInstance().AppearGoal();
	auto goal = GameManager::GetInstance().m_goal;
	auto cam = GameManager::GetInstance().m_camera;
	auto tar = goal->GetPosition();
	tar.y = 5.f;

	auto plForward = Vector3(0.f, 0.f, 1.f);
	auto camBeginPos = goal->GetPosition() + plForward * 65.f;
	auto camEndPos = goal->GetPosition() + plForward * 45.f;
	camBeginPos.y += 20.f;
	camEndPos.y += 20.f;
	auto sec = 1.5f;
	auto interval = 2.f;

	cam->Perform(
		camBeginPos, camEndPos,
		tar, tar, sec, interval
	);

}

void Game::Update()
{

	if (GameManager::GetInstance().m_stage->HasMapLoadingDone() and m_isCreateEnemyManager == false) {
		auto& eM = EnemyManager::GetEnemyManager();
		eM.SpawnEnemies();
		m_isCreateEnemyManager = true;

		//ついでにプレイヤーもここで生成・・・.
		GameManager::GetInstance().SpawnPlayer();

		GameManager::GetInstance().GetFade()->FadeIn();

	}

	if (m_boss != nullptr && !m_isBossCamPerform)
	{
		if (m_timer > 1.7f)
		{
			if (m_boss->GetCurrentState() == m_boss->GetState(TO_INT(IEnemy::EnState::enDeadState))) {
				GameObject* enemy = reinterpret_cast<GameObject*>(m_boss);
				EnemyManager::GetEnemyManager().DestroyEnemy(m_boss);
				GameManager::GetInstance().m_camera->ChangePlayerCam();
				m_isCalledExplode = false;
			}
			else {
				GameManager::GetInstance().m_camera->ChangeStateToEnemyCam();
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
		if (m_timer > 0.1 and !m_isCalledExplode) {
			if (m_boss->GetCurrentState() == m_boss->GetState(TO_INT(IEnemy::EnState::enDeadState))) {
				m_boss->Explode();
				m_isCalledExplode = true;
			}
		}

		m_timer += gameTime()->GetDeltaTime();
	}

	if (m_isGenerateGoalAfterBossPerformance) {
		m_goalAppearTimer += gameTime()->GetDeltaTime();
		if (m_goalAppearTimer >= m_goalAppearTime) {
			GoalGatePerformance();
			m_isGenerateGoalAfterBossPerformance = false;
		}
	}

	//if (g_pad[0]->IsTrigger(enButtonSelect)) {
	//	GoalGatePerformance();
	//}

	if (!GameManager::GetInstance().m_menu->IsGamePaused()) {
		m_clearTimer += gameTime()->GetDeltaTime();
	}

	//printf("Clear Timer : %f\n", m_clearTimer);

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
