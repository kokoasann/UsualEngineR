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
	//GameManager::GetInstance().GetPlayer()->SetState(GameManager::GetInstance().GetPlayer()->GetState(Player::EnState::))
	m_movie = NewGO<EventMovie>(0);
	m_movie->Init("Assets/eventMovie/ed.evm", g_camera3D,
		[&](const std::string& name)->ModelRender*
		{
			ModelRender* res = nullptr;
			if (std::strcmp(name.c_str(), "exit")==0)
			{
				m_exit = GameManager::GetInstance().m_goal->GetModelRender();
				res = m_exit;
			}
			else if (std::strcmp(name.c_str(), "m") == 0)
			{
				//GameManager::GetInstance().GetPlayer()->SetActive(false);
				m_player = &GameManager::GetInstance().GetPlayer()->GetModelRender();
				res = m_player;
			}
			else if (std::strcmp(name.c_str(), "Plane") == 0)
			{
				m_plane = NewGO<ModelRender>(0);
				ModelInitData mid;
				mid.m_tkmFilePath = "Assets/modelData/plane/plane.tkm";
				
				res = m_plane;
			}
			return res;
		},
		[&](const std::string& name)
		{

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
