#include "stdafx.h"
#include "Enemy/IEnemy.h"
#include "Enemy/State/IEnemyState.h"
#include "Enemy/EnemyManager.h"
#include "OpeningScene.h"
#include "EventMovie/EventMovie.h"
#include "Enemy/Boss/BossA.h"
#include "Enemy/Boss/Boss_Fatman.h"
#include "Enemy/Boss/Boss_MiddleBomb.h"
#include "Effect/MuzzleFlash.h"
#include "Effect/Beam.h"
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

	DeleteGO(m_eventMovie);
	for (auto mr : m_modelRenders) {
		DeleteGO(mr);
	}
	g_camera3D->SetViewAngle(Math::DegToRad(60));

	for (auto effect : m_muzzleFlashEffects) {
		DeleteGO(effect);
	}

	for (auto effect : m_beamEffects) {
		DeleteGO(effect);
	}

	DeleteGO(m_model);
	//auto meleeIdleState = m_melee->GetStateMap().at(TO_INT(IEnemy::EnState::enIdleState));
	//m_melee->SetState(meleeIdleState);

}

void OpeningScene::OnDestroy()
{
	Release();
}


void OpeningScene::Awake()
{

	GameManager::GetInstance().SetGameState(GameManager::EnGameState::Loading);

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
	EnemyManager::GetEnemyManager().AddEnemy(melee);
	//fat
	auto fat = NewGO<Boss_Fatman>(0);
	fat->SetAbility(ab);
	EnemyManager::GetEnemyManager().SetFatBoss(fat);
	EnemyManager::GetEnemyManager().AddEnemy(fat);
	//bomb
	auto bomb = NewGO<Boss_MiddleBomb>(0);
	bomb->SetAbility(ab);
	EnemyManager::GetEnemyManager().SetBombBoss(bomb);
	EnemyManager::GetEnemyManager().AddEnemy(bomb);
}

bool OpeningScene::Start()
{
	ModelInitData mid;
	mid.m_tkmFilePath = "Assets/modelData/opField/opField.tkm";
	mid.m_upAxis = EUpAxis::enUpAxisY;
	m_model = NewGO<ModelRender>(0);
	m_model->Init(mid);
	m_model->SetScale(Vector3::One);
	Fade::GetInstance().FadeIn();

	auto player = GameManager::GetInstance().GetPlayer();
	player->SetStateSudden(player->GetState(Player::EnState::enMovie));

	m_eventMovie = NewGO<EventMovie>(0);

	ActorInitFunc actorInitFunc = [&](const std::string& name) {
		//printf("actor init : ");
		//printf(name.c_str());
		//puts("");
		auto modelRender = GetItsModelRender(name);
		return modelRender;
	};

	EventListennerFunc eventListenerFunc = [&](const std::string& name) {

		if (std::strcmp(name.c_str(), "start") == 0) {
			GameManager::GetInstance().GetPlayer()->PlayAnimation(Player::EnAnimation::enWalk);
		}

		if (std::strcmp(name.c_str(), "m_idle") == 0) {
			EnemyManager::GetEnemyManager().GetMeleeBoss()->PlayAnimation(IEnemy::EnAnimation::enIdle);
			GameManager::GetInstance().GetPlayer()->PlayAnimation(Player::EnAnimation::enIdle);
		}

		//lol panch = punch.
		if (std::strcmp(name.c_str(), "melee_panch") == 0) {
			CSoundSource* se = NewGO<CSoundSource>(0);
			se->Init(L"Assets/sound/chara/footstep2.wav");
			se->SetVolume(1);
			se->Play(false);
			EnemyManager::GetEnemyManager().GetMeleeBoss()->PlayAnimation(IEnemy::EnAnimation::enAttackA);
		}

		if (std::strcmp(name.c_str(), "m_take_on") == 0) {
			CSoundSource* se = NewGO<CSoundSource>(0);
			se->Init(L"Assets/sound/chara/punch_2_1.wav");
			se->SetVolume(1);
			se->Play(false);
		}

		if (std::strcmp(name.c_str(), "melee_idle") == 0) {
			EnemyManager::GetEnemyManager().GetMeleeBoss()->PlayAnimation(IEnemy::EnAnimation::enIdle);
		}

		if (std::strcmp(name.c_str(), "targeting") == 0) {
			auto bomb = EnemyManager::GetEnemyManager().GetBombBoss();
			bomb->PlayAnimation(TO_INT(IEnemy::EnAnimation::enAttackA));
			m_isTargeting = true;

			//beam
			const int NumBeamEffects = 2;
			for (int i = 0; i < NumBeamEffects; i++) {
				auto beamEff = NewGO<Beam>(0);
				BeamEffectInitData beid;
				beamEff->Init(beid);
				m_beamEffects.push_back(beamEff);
			}

			auto fat = EnemyManager::GetEnemyManager().GetFatBoss();

			m_beamEffects[0]->SetChange(true);
			m_beamEffects[0]->SetSca(Vector3::One * 0.01f);
			m_beamEffects[0]->SetIsLoop(true);

			m_beamEffects[1]->SetChange(true);
			m_beamEffects[1]->SetSca(Vector3::One * 0.01f);
			m_beamEffects[1]->SetIsLoop(true);

			m_beamEffects[0]->Play();
			m_beamEffects[1]->Play();

			{
				CSoundSource* se = NewGO<CSoundSource>(0);
				se->Init(L"Assets/sound/boss_fatman/charge.wav");
				se->SetVolume(1);
				se->Play(false);
			}

		}


		if (std::strcmp(name.c_str(), "fire") == 0) {
			CSoundSource* se = NewGO<CSoundSource>(0);
			se->Init(L"Assets/sound/shoot.wav");
			se->SetVolume(1.);
			se->Play(false);

			const int NumMazzleFlashes = 3;
			MuzzleFlashEffectInitData eid;
			for (int i = 0; i < NumMazzleFlashes; i++) {
				auto muzzleFlash = NewGO<MuzzleFlash>(0);
				muzzleFlash->Init(eid);
				m_muzzleFlashEffects.push_back(muzzleFlash);
			}

			//fat's arms
			const float muzzleFlashScale = 0.25f;
			auto fat = EnemyManager::GetEnemyManager().GetFatBoss();
			const auto& ArmLMat = fat->GetIK(TO_INT(IEnemy::EnIK::enFoot_L))->GetEffectorBone()->GetWorldMatrix();
			const auto& ArmRMat = fat->GetIK(TO_INT(IEnemy::EnIK::enFoot_R))->GetEffectorBone()->GetWorldMatrix();
			//L
			m_muzzleFlashEffects[0]->SetSca(Vector3::One * muzzleFlashScale);
			m_muzzleFlashEffects[0]->Play();
			m_muzzleFlashEffects[0]->SetPos(ArmLMat.GetTransrate());
			m_muzzleFlashEffects[0]->SetRot(ArmLMat.GetRotate());
			//R
			m_muzzleFlashEffects[1]->SetSca(Vector3::One * muzzleFlashScale);
			m_muzzleFlashEffects[1]->Play();
			m_muzzleFlashEffects[1]->SetPos(ArmRMat.GetTransrate());
			m_muzzleFlashEffects[1]->SetRot(ArmRMat.GetRotate());

			//Bomb
			auto bomb = EnemyManager::GetEnemyManager().GetBombBoss();
			const auto& muzzleMat = bomb->GetIK(TO_INT(IEnemy::EnIK::enArm_L))->GetEffectorBone()->GetWorldMatrix();
			m_muzzleFlashEffects[2]->SetSca(Vector3::One * muzzleFlashScale);
			m_muzzleFlashEffects[2]->Play();
			m_muzzleFlashEffects[2]->SetPos(muzzleMat.GetTransrate());
			m_muzzleFlashEffects[2]->SetRot(muzzleMat.GetRotate());

		}

		if (std::strcmp(name.c_str(), "black_out") == 0) {
			Fade::GetInstance().BlackOut();
		}

		if (std::strcmp(name.c_str(), "in") == 0) {
			Fade::GetInstance().MakeBright();
		}

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

	if (m_isTargeting) {
		auto fat = EnemyManager::GetEnemyManager().GetFatBoss();
		fat->GetIK(TO_INT(IEnemy::EnIK::enArm_L))->SetNextTarget(g_camera3D->GetPosition());
		fat->GetIK(TO_INT(IEnemy::EnIK::enArm_R))->SetNextTarget(g_camera3D->GetPosition());
		fat->GetIK(TO_INT(IEnemy::EnIK::enFoot_L))->SetNextTarget(g_camera3D->GetPosition());
		fat->GetIK(TO_INT(IEnemy::EnIK::enFoot_R))->SetNextTarget(g_camera3D->GetPosition());

		auto bomb = EnemyManager::GetEnemyManager().GetBombBoss();
		bomb->GetIK(TO_INT(IEnemy::EnIK::enArm_L))->SetNextTarget(g_camera3D->GetPosition());

		//
		const auto& ArmLMat = fat->GetIK(TO_INT(IEnemy::EnIK::enArm_L))->GetEffectorBone()->GetWorldMatrix();
		const auto& ArmRMat = fat->GetIK(TO_INT(IEnemy::EnIK::enArm_R))->GetEffectorBone()->GetWorldMatrix();

		m_beamEffects[0]->SetPos(ArmLMat.GetTransrate());
		m_beamEffects[0]->SetRot(ArmLMat.GetRotate());
		m_beamEffects[1]->SetPos(ArmRMat.GetTransrate());
		m_beamEffects[1]->SetRot(ArmRMat.GetRotate());
	}

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

