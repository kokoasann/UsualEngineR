#include "stdafx.h"
#include "EndingScene.h"

#include "EventMovie/EventMovie.h"

#include "GameManager.h"
#include "Player/Player.h"
#include "Goal.h"
#include "Fade.h"
#include "Camera/GameCamera.h"

EndingScene::EndingScene()
{

}

EndingScene::~EndingScene()
{

}



void EndingScene::Release()
{

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
			}
			else if (std::strcmp(name.c_str(), "FOV_30") == 0)
			{
				g_camera3D->SetViewAngle(Math::DegToRad(30));
			}
			else if (std::strcmp(name.c_str(), "light_exit") == 0)
			{
				//m_exit->SetMulColor({ 3,3,3,1 });
			}
			else if (std::strcmp(name.c_str(), "dark_exit") == 0)
			{
				m_exit->SetMulColor({ 1,1,1,1 });
			}
			else if (std::strcmp(name.c_str(), "open") == 0)
			{
				m_exit->Play(1);
			}
			else if (std::strcmp(name.c_str(), "fade_out") == 0)
			{
				Fade::GetInstance().SetFadeColor(Vector3::One);
				Fade::GetInstance().SetFadeSpeed(1. / 4.);
				Fade::GetInstance().FadeOut();
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
