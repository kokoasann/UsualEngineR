#include "stdafx.h"
#include "Result.h"
#include "Title.h"
#include "GameManager.h"
#include "Fade.h"

//#define _USE_KANJI

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

	Fade::GetInstance().FadeIn();

	printf("fade in\n");

	return true;
}


void Result::Init(const double clearTime, const int eliminateCount) {

	//Robots
	ModelInitData mid;
	mid.m_upAxis = EUpAxis::enUpAxisY;
	mid.m_tkmFilePath = "Assets/modelData/AssistantMachine/am.tkm";
	mid.m_tksFilePath = "Assets/modelData/AssistantMachine/am.tks";
	mid.m_vsfxFilePath = "Assets/shader/AnimModel.fx";

	const int NUM_ROBOT = 100;

	m_corpseCount = eliminateCount;

	m_clearSec = clearTime;
	float clearMin = clearTime / 60.0;
	//printf("clear time : %f(min)\n", clearTime / 60.0);
	const float WorstMin = 60.f;
	const float BestMin = WorstMin / 2.f;

	if (clearMin >= WorstMin) {
		m_displayRoboCount = 1;
	}
	else if (clearMin <= BestMin) {
		m_displayRoboCount = NUM_ROBOT;
	}
	else {
		m_displayRoboCount = NUM_ROBOT - (NUM_ROBOT * (clearMin - BestMin) / BestMin);
	}


	m_level.Init("Assets/level/resultPOWs_level.tkl", [&](LevelObjectData& objData)->bool
		{
			if (m_robotCount >= m_displayRoboCount) return true;
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
			m_robotCount++;
			//printf("robo count : %d\n", robotCount);
			return true;
		});

}


void Result::PreUpdate()
{

}

void Result::Update()
{

	if (g_pad[0]->IsTrigger(EnButton::enButtonA)) {
		Fade::GetInstance().FadeOut();
		m_isTriggeredButtonA =  true;
	}

	if (Fade::GetInstance().IsFaded() and m_isTriggeredButtonA) {
		NewGO<Title>(0);
		auto go = reinterpret_cast<GameObject*>(this);
		DeleteGO(go);
	}
}

void Result::PostUpdate()
{
	Fade::GetInstance().Update();
}


void Result::Render()
{

}

void Result::PostRender()
{

	static Font font;
	static const Vector4 FontColor = Vector4::White;
	static const Vector2 timePos = { -500.f, 300.f };
	static const float rot = 0.f;
	static const float sca = 1.f;

	//TIME
	const int clearMin = (int)m_clearSec / 60.f;
	const int clearHour = (clearMin / 60.f) >= 1.f ? (int)(clearMin / 60.f) : 0;
	const int sec = max(0, (int)m_clearSec - (clearMin * 60.0));
	//std::wstring time = L"Time  " + std::to_wstring(clearHour) + L":" + std::to_wstring((int)(clearMin - clearHour * 60.0)) + L":" + std::to_wstring(sec);


#ifndef _USE_KANJI
	std::wstring time = L"Time " + std::to_wstring(clearMin) + L":" + std::to_wstring(sec) + L"\n";
	std::wstring numPow = L"POW " + std::to_wstring(m_displayRoboCount) + L"\n";
	std::wstring eliminatedCount = L"Corpse " + std::to_wstring(m_corpseCount) + L"\n";
#else
	std::wstring time = L"Time " + std::to_wstring(clearMin) + L"ï™" + std::to_wstring(sec) + L"ïb\n";
	std::wstring numPow = L"ïﬂó∏êî " + std::to_wstring(m_displayRoboCount)  + L"\n";
	std::wstring eliminatedCount = L"åÇîjêî " + std::to_wstring(m_corpseCount) + L"\n";
#endif

	//
	std::wstring text = time + numPow + eliminatedCount;
	font.Begin();
	font.Draw(text.c_str(), timePos, FontColor, rot, sca);
	font.End();


}
