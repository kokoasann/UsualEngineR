#include "stdafx.h"
#include "Result.h"
#include "Title.h"

Result::Result()
{

}

Result::~Result()
{

}



void Result::Release()
{
	for (auto robot : m_robots) {
		DeleteGO(robot);
	}

	DeleteGO(m_ground);
	DeleteGO(m_backSprite);
}

void Result::OnDestroy()
{
	Release();
}


void Result::Awake()
{

}

bool Result::Start()
{
	ModelInitData mid;
	mid.m_upAxis = EUpAxis::enUpAxisY;
	mid.m_tkmFilePath = "Assets/modelData/AssistantMachine/am.tkm";
	mid.m_tksFilePath = "Assets/modelData/AssistantMachine/am.tks";
	mid.m_vsfxFilePath = "Assets/shader/AnimModel.fx";

	//Ground
	m_ground = NewGO<ModelRender>(0);
	m_ground->Init(mid);
	m_ground->SetScale(Vector3(1000.f,0.5f,1000.f));
	m_ground->SetPosition(Vector3(0.f,-5.f,0.f));

	g_camera3D->SetPosition(m_resultCameraPos);
	g_camera3D->SetTarget(m_resultCameraTarget);

	//Sprite
	SpriteInitData sd;
	sd.m_ddsFilePath[0] = "Assets/Image/targetMarker.dds";
	sd.m_height = 256;
	sd.m_width = 256;
	m_backSprite = NewGO<SpriteRender>(0);
	m_backSprite->Init(sd);
	m_backSprite->SetPos(m_backSpPos);
	m_backSprite->SetSca(m_backSpScale);

	return true;
}


void Result::Init(const double clearTime) {
	//Robots
	ModelInitData mid;
	mid.m_upAxis = EUpAxis::enUpAxisY;
	mid.m_tkmFilePath = "Assets/modelData/AssistantMachine/am.tkm";
	mid.m_tksFilePath = "Assets/modelData/AssistantMachine/am.tks";
	mid.m_vsfxFilePath = "Assets/shader/AnimModel.fx";

	int robotCount = 0;
	const int NUM_ROBOT = 100;
	int displayRoboCount = 0;

	float clearMin = clearTime / 60.0;
	//printf("clear time : %f(min)\n", clearTime / 60.0);
	const float WorstMin = 60.f;
	const float BestMin = WorstMin / 2.f;

	if (clearMin >= WorstMin) {
		displayRoboCount = 1;
	}
	else if (clearMin <= BestMin) {
		displayRoboCount = NUM_ROBOT;
	}
	else {
		displayRoboCount = NUM_ROBOT - (NUM_ROBOT * (clearMin - BestMin) / BestMin);
	}


	m_level.Init("Assets/level/resultPOWs_level.tkl", [&](LevelObjectData& objData)->bool
		{
			if (robotCount >= displayRoboCount) return true;
			std::string name(objData.name.begin(), objData.name.end());
			ModelRender* mr = NewGO<ModelRender>(0);
			mr->SetScale(Vector3::One * m_levelScale);
			mid.m_tkmFilePath = "Assets/modelData/AssistantMachine/am.tkm";
			mr->Init(mid);
			mr->SetScale(objData.scale * m_levelScale);
			mr->SetPosition(objData.position * m_levelScale);
			mr->SetRotation(objData.rotation);
			m_robots.push_back(mr);
			mr->SetMulColor({ 0.4,0.4,0.4,1 });
			robotCount++;
			printf("robo count : %d\n", robotCount);
			return true;
		});

}


void Result::PreUpdate()
{

}

void Result::Update()
{
	if (g_pad[0]->IsTrigger(EnButton::enButtonA)) {
		NewGO<Title>(0);
		auto go = reinterpret_cast<GameObject*>(this);
		DeleteGO(go);
	}
}

void Result::PostUpdate()
{

}


void Result::Render()
{

}

void Result::PostRender()
{

}
