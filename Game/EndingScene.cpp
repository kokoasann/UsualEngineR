#include "stdafx.h"
#include "EndingScene.h"

#include "EventMovie/EventMovie.h"

#include "GameManager.h"
#include "Game.h"
#include "Player/Player.h"
#include "Goal.h"
#include "Fade.h"
#include "Camera/GameCamera.h"

#include "Enemy/EnemyManager.h"
#include "Result.h"
#include "Player/Player.h"
#include "Player/Attachment/JetPack.h"
#include "Player/Attachment/Gun.h"
#include "Player/Attachment/Shield.h"

EndingScene::EndingScene()
{

}

EndingScene::~EndingScene()
{

}



void EndingScene::Release()
{
	DeleteGO(m_edBack);
	DeleteGO(m_edBack_cross);
	DeleteGO(m_esca);
	DeleteGO(m_gogogo);
	DeleteGO(m_movie);
	DeleteGO(m_plane);
	DeleteGO(GameManager::GetInstance().m_player);
}

void EndingScene::OnDestroy()
{
	Release();
}


void EndingScene::Awake()
{

}

bool EndingScene::Start()
{
	GameManager::GetInstance().GetGameCamera()->SetActive(false);
	GameManager::GetInstance().GetPlayer()->SetState(GameManager::GetInstance().GetPlayer()->GetState(Player::EnState::enMovie));
	
	
	ModelInitData mid;
	mid.m_upAxis = enUpAxisY;
	mid.m_tkmFilePath = "Assets/modelData/edBack/edBack.tkm";
	m_edBack = NewGO<ModelRender>(0);
	m_edBack->Init(mid);
	m_edBack->SetMulColor({ 0,0,0,1 });
	mid.m_tkmFilePath = "Assets/modelData/edBack/edBack_cross.tkm";
	m_edBack_cross = NewGO<ModelRender>(0);
	m_edBack_cross->Init(mid);
	//m_edBack_cross->SetMulColor({ 11,11,11,1 });
	m_edBack_cross->SetMulColor({ 50,1,1,1 });
	g_graphicsEngine->GetPostEffect().GetBloom().SetStreekFrag(false);

	m_esca = NewGO<CSoundSource>(0);
	m_esca->Init(L"Assets/sound/esca.wav");
	m_gogogo = NewGO<CSoundSource>(0);
	m_gogogo->Init(L"Assets/sound/gogogo.wav");
	//m_exit->SetAnimPlaySpeed(0.5);

	m_movie = NewGO<EventMovie>(0);
	m_movie->Init("Assets/eventMovie/ed.evm", g_camera3D,
		[&](const std::string& name)->ModelRender*
		{
			ModelRender* res = nullptr;
			if (std::strcmp(name.c_str(), "exit")==0)
			{
				GameManager::GetInstance().m_goal->SetActive(false);
				m_exit = GameManager::GetInstance().m_goal->GetModelRender();
				m_exit->SetAnimPlaySpeed(0.5);
				m_exit->Play(0);
				res = m_exit;
			}
			else if (std::strcmp(name.c_str(), "m") == 0)
			{
				GameManager::GetInstance().GetPlayer()->SetActive(false);
				GameManager::GetInstance().GetPlayer()->PlayAnimation(Player::EnAnimation::enIdle);
				m_player = &GameManager::GetInstance().GetPlayer()->GetModelRender();
				
				res = m_player;
			}
			else if (std::strcmp(name.c_str(), "Plane") == 0)
			{
				m_plane = NewGO<ModelRender>(0);
				ModelInitData mid;
				mid.m_upAxis = enUpAxisY;
				mid.m_tkmFilePath = "Assets/modelData/plane/plane.tkm";
				m_plane->Init(mid);
				m_plane->SetMulColor({ 50,50,50,1 });
				res = m_plane;
			}
			return res;
		},
		[&](const std::string& name)
		{
			if (std::strcmp(name.c_str(), "FOV_60")==0)
			{
				g_camera3D->SetViewAngle(Math::DegToRad(60));
				
				if (!m_isTake_on)
				{
					m_esca->SetVolume(0.5);
					m_esca->Play(true);
				}
			}
			else if (std::strcmp(name.c_str(), "FOV_30") == 0)
			{
				g_camera3D->SetViewAngle(Math::DegToRad(30));
			}
			else if (std::strcmp(name.c_str(), "light_exit") == 0)
			{
				m_esca->SetVolume(0.2);
				//m_exit->SetMulColor({ 3,3,3,1 });
			}
			else if (std::strcmp(name.c_str(), "dark_exit") == 0)
			{
				m_esca->SetVolume(0.5);
				m_exit->SetMulColor({ 1,1,1,1 });
			}
			else if (std::strcmp(name.c_str(), "take_on") == 0)
			{
				m_esca->Stop();
				m_isTake_on = true;
				CSoundSource* se = NewGO<CSoundSource>(0);
				se->Init(L"Assets/sound/omoi_panchi.wav");
				se->SetVolume(0.5);
				se->Play(false);
			}
			else if (std::strcmp(name.c_str(), "open") == 0)
			{
				m_exit->Play(1);
				m_gogogo->Play(true);
				m_gogogo->SetVolume(0.3);
			}
			else if (std::strcmp(name.c_str(), "fade_out") == 0)
			{
				Fade::GetInstance().SetFadeColor(Vector3::One);
				Fade::GetInstance().SetFadeSpeed(1. / 4.);
				Fade::GetInstance().FadeOut();
				m_isFadeOut = true;
			}
		});
	
	Fade::GetInstance().FadeIn();

	return true;
}


void EndingScene::PreUpdate()
{

}

void EndingScene::Update()
{
	float dtime = gameTime()->GetDeltaTime();
	if (m_isFadeOut)
	{
		float v = m_gogogo->GetVolume();
		if (v == 0.f)
		{
			m_gogogo->Stop();
		}
		else
			m_gogogo->SetVolume(max(v - dtime * (1. / 8.),0.f));

		if (Fade::GetInstance().IsFaded())
		{
			


			auto result = NewGO<Result>(0);

			const double meleeActiveTime = GameManager::GetInstance().m_player->GetJetPack()->GetActivatedTime();
			const double remoteActiveTime = GameManager::GetInstance().m_player->GetGun()->GetActivatedTime();
			const double bombActiveTime = GameManager::GetInstance().m_player->GetShield()->GetActivatedTime();

			Result::SAttachmentPercentage ap;
			ap.defaultAttachment = ((m_clearTimer - (meleeActiveTime + remoteActiveTime + bombActiveTime)) / m_clearTimer) * 100.0;
			ap.meleeAttachment = (meleeActiveTime / m_clearTimer) * 100.0;
			ap.remoteAttachment = (remoteActiveTime / m_clearTimer) * 100.0;
			ap.bombAttachment = (bombActiveTime / m_clearTimer) * 100.0;
			const int numCorpse = EnemyManager::GetEnemyManager().GetCorpseCount();
			result->Init(m_clearTimer, numCorpse, ap);

			DeleteGO(GameManager::GetInstance().m_gameScene);
			auto& fade = Fade::GetInstance();
			fade.SetFadeSpeed(1);
			fade.SetFadeColor({ 0.f,0.f,0.f});

			auto pthis = this;
			DeleteGO(pthis);
		}
	}
}

void EndingScene::PostUpdate()
{

}


void EndingScene::Render()
{

}

void EndingScene::PostRender()
{

}
