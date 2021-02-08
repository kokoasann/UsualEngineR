#include "stdafx.h"
#include "OpeningScene.h"
#include "EventMovie/EventMovie.h"

OpeningScene::OpeningScene()
{

}

OpeningScene::~OpeningScene()
{

}



void OpeningScene::Release()
{
	DeleteGO(m_eventMovie);
}

void OpeningScene::OnDestroy()
{
	Release();
}


void OpeningScene::Awake()
{

}

bool OpeningScene::Start()
{

	InitModelRenderMap();

	m_eventMovie = NewGO<EventMovie>(0);

	ActorInitFunc actorInitFunc = [&](const std::string& name){
		printf("actor init : ");
		printf(name.c_str());
		puts("");

		auto model = NewGO<ModelRender>(0);
		ModelInitData mid;
		//mid.m_tkmFilePath = L"Assets/modelData/"
		//model->Init()
		return model;
	};

	EventListennerFunc eventListenerFunc = [&](const std::string& name){
		printf("Event Listener : ");
		printf(name.c_str());
		puts("");

		return;
	};

	m_eventMovie->Init("Assets/eventMovie/op.evm", g_camera3D,actorInitFunc,eventListenerFunc);

	return true;
}


void OpeningScene::PreUpdate()
{

}

void OpeningScene::Update()
{

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

void OpeningScene::InitModelRenderMap() {

	ModelInitData mid;
	mid.m_vsfxFilePath = "Assets/shader/AnimModel.fx";
	mid.m_upAxis = EUpAxis::enUpAxisY;

	{
		auto modelR = NewGO<ModelRender>(0);
		mid.m_tkmFilePath = "Assets/modelData/m/m_ExBone.tkm";
		mid.m_tksFilePath = "Assets/modelData/m/m_ExBone.tks";
		modelR->Init(mid);
		m_modelRenderMap.insert(std::make_pair("m", modelR));
	}

}