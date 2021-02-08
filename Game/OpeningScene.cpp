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
	for (auto mr : m_modelRenders) {
		DeleteGO(mr);
	}
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

	m_eventMovie = NewGO<EventMovie>(0);

	ActorInitFunc actorInitFunc = [&](const std::string& name){
		//printf("actor init : ");
		//printf(name.c_str());
		//puts("");
		auto modelRender = CreateModelRender(name);
		m_modelRenders.push_back(modelRender);
		return modelRender;
	};

	EventListennerFunc eventListenerFunc = [&](const std::string& name){
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

ModelRender* OpeningScene::CreateModelRender(const std::string& name) {

	ModelInitData mid;
	mid.m_vsfxFilePath = "Assets/shader/AnimModel.fx";
	mid.m_upAxis = EUpAxis::enUpAxisZ;
	auto modelR = NewGO<ModelRender>(0);

	//chara
	if (std::strcmp(name.c_str(), "m") == 0) {
		//printf("create m\n");
		mid.m_tkmFilePath = "Assets/modelData/m/m_ExBone.tkm";
		mid.m_tksFilePath = "Assets/modelData/m/m_ExBone.tks";
		modelR->Init(mid);
		m_chara = modelR;
	}else 
	//fat
	if (std::strcmp(name.c_str(), "fat") == 0) {
		//printf("create fat\n");
		mid.m_tkmFilePath = "Assets/modelData/boss/lf/lf.tkm";
		mid.m_tksFilePath = "Assets/modelData/boss/lf/lf.tks";
		modelR->Init(mid);
		m_fat = modelR;
	}else
	//bomb
	if (std::strcmp(name.c_str(), "bomb") == 0) {
		//printf("create bomb\n");
		mid.m_tkmFilePath = "Assets/modelData/boss/mb/mb.tkm";
		mid.m_tksFilePath = "Assets/modelData/boss/mb/mb.tks";
		modelR->Init(mid);
		m_bomb = modelR;
	}else
	//melee
	if (std::strcmp(name.c_str(), "melee") == 0) {
		//printf("create melee\n");
		mid.m_tkmFilePath = "Assets/modelData/boss/sp/sp.tkm";
		mid.m_tksFilePath = "Assets/modelData/boss/sp/sp.tks";
		modelR->Init(mid);
		m_melee = modelR;
	}else
	//pod
	if (contain(name, "pod")) {
		//printf("create pod\n");
		mid.m_tkmFilePath = "Assets/modelData/AssistantMachine/am.tkm";
		mid.m_tksFilePath = "Assets/modelData/AssistantMachine/am.tks";
		modelR->Init(mid);
		m_pods.push_back(modelR);
	}

	return modelR;
}

