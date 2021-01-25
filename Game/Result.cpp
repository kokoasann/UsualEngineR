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
	//Robots
	ModelInitData mid;
	mid.m_tkmFilePath = "Assets/modelData/AssistantMachine/am.tkm";
	mid.m_tksFilePath = "Assets/modelData/AssistantMachine/am.tks";
	mid.m_upAxis = EUpAxis::enUpAxisY;
	mid.m_vsfxFilePath = "Assets/shader/AnimModel.fx";

	Vector3 pos = Vector3::Zero;

	for (int i = 0; i < m_numRobot; i++) {
		auto robot = NewGO<ModelRender>(0);
		robot->Init(mid);
		robot->SetScale(m_scale);
		robot->SetPosition(pos);
		pos.z -= 5.f;
		m_robots.push_back(robot);
	}

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


void Result::PreUpdate()
{

}

void Result::Update()
{
	if (g_pad[0]->IsTrigger(EnButton::enButtonA)) {
		NewGO<Title>(0);
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
