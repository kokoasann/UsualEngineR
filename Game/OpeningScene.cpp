#include "stdafx.h"
#include "Enemy/IEnemy.h"
#include "Enemy/State/IEnemyState.h"
#include "Enemy/EnemyManager.h"
#include "OpeningScene.h"
#include "EventMovie/EventMovie.h"
#include "Enemy/Boss/BossA.h"
#include "Enemy/Boss/Boss_Fatman.h"
#include "Enemy/Boss/Boss_MiddleBomb.h"
#include "Player/Player.h"
#include "Game.h"
#include "Fade.h"


OpeningScene::OpeningScene()
{
}

OpeningScene::~OpeningScene()
{

}



void OpeningScene::Release()
{
	GameManager::GetInstance().SetGameState(GameManager::EnGameState::InGame);
	GameManager::GetInstance().SpawnPod();

	DeleteGO(m_eventMovie);
	for (auto mr : m_modelRenders) {
		DeleteGO(mr);
	}
	g_camera3D->SetViewAngle(Math::DegToRad(60));

	//auto meleeIdleState = m_melee->GetStateMap().at(TO_INT(IEnemy::EnState::enIdleState));
	//m_melee->SetState(meleeIdleState);

}

void OpeningScene::OnDestroy()
{
	Release();
}


void OpeningScene::Awake()
{
	//chara 
	GameManager::GetInstance().SpawnPlayer();

	//m_chara = NewGO<Player>(0);

	//enemy
	IEnemy::StAbility ab;
	ab.InitHP(1000);
	ab.InitStamina(1000);
	//melee
	auto melee = NewGO<BossA>(0);
	melee->SetAbility(ab);
	EnemyManager::GetEnemyManager().SetMeleeBoss(melee);
	//fat
	auto fat = NewGO<Boss_Fatman>(0);
	fat->SetAbility(ab);
	EnemyManager::GetEnemyManager().SetFatBoss(fat);
	//bomb
	auto bomb = NewGO<Boss_MiddleBomb>(0);
	bomb->SetAbility(ab);
	EnemyManager::GetEnemyManager().SetBombBoss(bomb);
}

bool OpeningScene::Start()
{
	Fade::GetInstance().FadeIn();

	m_eventMovie = NewGO<EventMovie>(0);

	ActorInitFunc actorInitFunc = [&](const std::string& name) {
		//printf("actor init : ");
		//printf(name.c_str());
		//puts("");
		auto modelRender = GetItsModelRender(name);
		return modelRender;
	};

	EventListennerFunc eventListenerFunc = [&](const std::string& name) {
		if (std::strcmp(name.c_str(), "fade_out") == 0) {
			Fade::GetInstance().FadeOut();
			m_isFadingToGame = true;
		}
		return;
	};

	g_camera3D->SetViewAngle(Math::DegToRad(30));	// I‚í‚Á‚½‚ç60‚É’¼‚µ‚Ä‚Ë!!
	m_eventMovie->Init("Assets/eventMovie/op.evm", g_camera3D, actorInitFunc, eventListenerFunc);
	return true;
}


void OpeningScene::PreUpdate()
{

}

void OpeningScene::Update()
{
	if (m_isFadingToGame and Fade::GetInstance().IsFaded()) {
		auto opobj = reinterpret_cast<GameObject*>(this);
		DeleteGO(opobj);
		NewGO<Game>(0);
	}
	Fade::GetInstance().Update();
}

void OpeningScene::PostUpdate()
{
}


void OpeningScene::Render()
{

}

void OpeningScene::PostRender()
{

}

ModelRender* OpeningScene::GetItsModelRender(const std::string& name) {

	static const float BOSS_A_HP = 1500.f;
	static const float BOSS_A_STAMINA = 100.f;

	ModelInitData mid;
	//mid.m_vsfxFilePath = "Assets/shader/NoAnimModel.fx";
	mid.m_upAxis = EUpAxis::enUpAxisY;
	ModelRender* modelR = nullptr;

	auto fatboss = EnemyManager::GetEnemyManager().GetFatBoss();
	//chara
	if (std::strcmp(name.c_str(), "m") == 0) {
		modelR = &GameManager::GetInstance().GetPlayer()->GetModelRender();
	}else 
	//fat
	if (std::strcmp(name.c_str(), "fat") == 0) {
		return EnemyManager::GetEnemyManager().GetFatBoss()->GetModel();
	}else
	//bomb
	if (std::strcmp(name.c_str(), "bomb") == 0) {
		return EnemyManager::GetEnemyManager().GetBombBoss()->GetModel();
	}else
	//melee
	if (std::strcmp(name.c_str(), "melee") == 0) {
		//printf("create melee\n");
		return EnemyManager::GetEnemyManager().GetMeleeBoss()->GetModel();
	}else
	//pod
	if (contain(name, "pod")) {
		//printf("create pod\n");
		modelR = NewGO<ModelRender>(0);
		mid.m_tkmFilePath = "Assets/modelData/AssistantMachine/am.tkm";
		mid.m_tksFilePath = "Assets/modelData/AssistantMachine/am.tks";
		modelR->Init(mid);
		m_modelRenders.push_back(modelR);
		m_pods.push_back(modelR);
	}

	return modelR;
}

