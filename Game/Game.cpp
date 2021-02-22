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
#include "Player/Player.h"
#include "Player/Attachment/JetPack.h"
#include "Player/Attachment/Gun.h"
#include "Player/Attachment/Shield.h"

#include "EndingScene.h"

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
	m_windSE = NewGO < CSoundSource>(0);
	m_windSE->Init(L"Assets/sound/windoOo.wav", false);
	m_windSE->Play(true);
}

void Game::OnGoal() {
	Fade::GetInstance().FadeOut();
	m_isCleared = true;
	DebugPrint_WATA("goal\n");
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
		const int NUM_BOSS = 3;
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
	GameManager::GetInstance().ResetDeadBossCount();
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
	auto camBeginPos = goal->GetPosition() + plForward * 105.f;
	auto camEndPos = goal->GetPosition() + plForward * 65.f;
	camBeginPos.y = 40.f;
	camEndPos.y = 40.f;
	auto sec = 3.0f;
	auto interval = 2.f;

	Quaternion rot = Quaternion::Identity;
	rot.SetRotationDegY(90.f);
	rot.Apply(camBeginPos);
	rot.Apply(camEndPos);

	cam->Perform(
		camBeginPos, camEndPos,
		tar, tar, sec, interval
	);

}

void Game::Update()
{

	// GOAL DEBUG
	//if (g_pad[0]->IsTrigger(EnButton::enButtonB)) {
		//GoalGatePerformance();
	//}

	if (GameManager::GetInstance().m_stage->HasMapLoadingDone() and m_isCreateEnemyManager == false) {
		auto& eM = EnemyManager::GetEnemyManager();
		eM.SpawnEnemies();
		m_isCreateEnemyManager = true;

		//ついでにプレイヤーもここで生成・・・.
		GameManager::GetInstance().SpawnPlayer();
		GameManager::GetInstance().SpawnPod();

		Fade::GetInstance().FadeIn();
	}

	if (m_boss != nullptr && !m_isBossCamPerform)
	{
		if (m_timer > 1.7f)
		{
			if (m_boss->GetCurrentState() == m_boss->GetState(TO_INT(IEnemy::EnState::enDeadState))) {
				m_boss->SpawnDropItem();
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

	if (!GameManager::GetInstance().m_menu->IsGamePaused() and !m_isCleared) {
		m_clearTimer += gameTime()->GetDeltaTime();
	}

	//printf("Clear Timer : %f\n", m_clearTimer);

	auto& fade = Fade::GetInstance();
	if (fade.IsFaded() and m_isCleared and !m_engingFlag) {
		//auto result = NewGO<Result>(0);
		
		const double meleeActiveTime = GameManager::GetInstance().m_player->GetJetPack()->GetActivatedTime();
		const double remoteActiveTime = GameManager::GetInstance().m_player->GetGun()->GetActivatedTime();
		const double bombActiveTime = GameManager::GetInstance().m_player->GetShield()->GetActivatedTime();

		Result::SAttachmentPercentage ap;
		ap.defaultAttachment = ((m_clearTimer - (meleeActiveTime + remoteActiveTime + bombActiveTime)) / m_clearTimer) * 100.0;
		ap.meleeAttachment =  (meleeActiveTime / m_clearTimer) * 100.0;
		ap.remoteAttachment =  (remoteActiveTime / m_clearTimer) * 100.0;
		ap.bombAttachment = (bombActiveTime / m_clearTimer) * 100.0;

		//printf("Clear Time : %f\n default Time : %f (%f%%)\n melee Time : %f (%f%)\n remote Time : %f (%f%)\n bomb Time : %f (%f%)\n",
		//	m_clearTimer,
		//	m_clearTimer - (meleeActiveTime + remoteActiveTime + bombActiveTime), (m_clearTimer - (meleeActiveTime + remoteActiveTime + bombActiveTime)) / m_clearTimer,
		//	meleeActiveTime, meleeActiveTime / m_clearTimer,
		//	remoteActiveTime, remoteActiveTime / m_clearTimer,
		//	bombActiveTime, bombActiveTime / m_clearTimer
		//);

		const int numCorpse = EnemyManager::GetEnemyManager().GetCorpseCount();
		//result->Init(m_clearTimer, numCorpse, ap);
		auto go = reinterpret_cast<GameObject*>(this);
		//DeleteGO(go);

		DeleteGO(GameManager::GetInstance().m_stage);
		EnemyManager::GetEnemyManager().Release();
		DeleteGO(GameManager::GetInstance().m_gameHUD);
		//DeleteGO(GameManager::GetInstance().m_itemManager);

		EndingScene* end = NewGO<EndingScene>(0);
		end->Init(m_clearTimer);
		m_engingFlag = true;
		SetActive(false);

		if (m_windSE != nullptr) {
			m_windSE->Stop();
			DeleteGO(m_windSE);
			m_windSE = nullptr;
		}
	}
}

void Game::PostUpdate()
{
	Fade::GetInstance().Update();

	{
		auto player = GameManager::GetInstance().m_player;
		float HP = player->GetCurrentHP();
		//体力0以下ならポーズさせない
		if (HP <= 0.f)
			return;
		if (g_pad[0]->IsTrigger(EnButton::enButtonStart) and !GameManager::GetInstance().GetGameCamera()->IsPerforming()) {
			if (GameManager::GetInstance().m_menu->IsGamePaused()) {
				GameManager::GetInstance().m_menu->ResumeGame();
			}
			else {
				GameManager::GetInstance().m_menu->PauseGame();
			}
		}
	}

	if (Fade::GetInstance().IsFaded() and m_restartFlag) {
		auto go = reinterpret_cast<GameObject*>(this);
		DeleteGO(go);
		NewGO<Game>(0);
	}

	if (Fade::GetInstance().IsFaded() and m_toTitleFlag) {
		auto player = GameManager::GetInstance().GetPlayer();
		player->SetStateSudden(player->GetState(Player::EnState::enMovie));
		auto go = reinterpret_cast<GameObject*>(this);
		DeleteGO(go);
		NewGO<Title>(1);
	}
}

void Game::Render()
{
}

void Game::PostRender()
{
	/*static Font font;
	font.Begin();
	font.Draw(L"テストてすとTESTtest", { 0,0 }, Vector4::White, 0, 1);
	font.End();*/
}


void Game::Restart() {
	Fade::GetInstance().FadeOut();
	m_restartFlag = true;
	if (m_windSE != nullptr) {
		m_windSE->Stop();
		DeleteGO(m_windSE);
		m_windSE = nullptr;
	}
}

void Game::ToTitle() {
	Fade::GetInstance().FadeOut();
	m_toTitleFlag = true;
	if (m_windSE != nullptr) {
		m_windSE->Stop();
		DeleteGO(m_windSE);
		m_windSE = nullptr;
	}
}