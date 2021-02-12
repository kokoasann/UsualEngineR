#include "stdafx.h"
#include "Result.h"
#include "Title.h"
#include "GameManager.h"
#include "Fade.h"

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

	for (auto back : m_attachBack)
	{
		DeleteGO(back);
	}
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
	sd.m_ddsFilePath[0] = "Assets/Image/resultBack.dds";
	sd.m_height = 512;
	sd.m_width = 1260;
	m_backSprite = NewGO<SpriteRender>(0);
	m_backSprite->Init(sd);
	m_backSprite->SetPos(m_backSpPos);
	m_backSprite->SetSca(m_backSpScale);


	sd.m_ddsFilePath[0] = "Assets/Image/white.dds";
	sd.m_height = 16;
	sd.m_width = 16;
	for (int i = 0; i < 4; i += 1) {
		m_attachBack[i] = NewGO<SpriteRender>(0);
		m_attachBack[i]->Init(sd);
		m_attachBack[i]->SetPivot({ 1,1 });
		m_attachBack[i]->SetSca({ 17,3.5,0 });
		m_attachBack[i]->SetMulColor(m_attachColor[i]);
		m_attachBack[i]->SetPos({ X3, Y1 - (RightYSpace * (i*2+1))+5.f ,0 });
		//m_attachBack[i]->SetMulColor({})
	}

	Fade::GetInstance().FadeIn();

	printf("fade in\n");

	return true;
}


void Result::Init(const double clearTime, const int eliminateCount, const SAttachmentPercentage data) {

	//Robots
	ModelInitData mid;
	mid.m_upAxis = EUpAxis::enUpAxisY;
	mid.m_tkmFilePath = "Assets/modelData/AssistantMachine/am.tkm";
	mid.m_tksFilePath = "Assets/modelData/AssistantMachine/am.tks";
	mid.m_vsfxFilePath = "Assets/shader/AnimModel.fx";

	const int NUM_ROBOT = 100;

	m_attachmentPercentageData = data;

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

	if (g_pad[0]->IsTrigger(EnButton::enButtonA) and !m_isTriggeredButtonA) {
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
	//TIME
	//LEFT
	for (int i = 0; i < 6; i+=2) {
		m_fontsPos[i] = Vector2(X1, Y1 - (LeftYSpace * i));
	}
	for (int i = 1; i < 6; i += 2) {
		m_fontsPos[i] = Vector2(X2, Y1 - (LeftYSpace * (i - 1)));
	}
	//RIGHT
	for (int i = 6; i < m_NUM_FONTS; i+=2) {
		m_fontsPos[i] = Vector2(X3, Y1 - (RightYSpace * (i - 6)));
	}
	for (int i = 7; i < m_NUM_FONTS; i += 2) {
		m_fontsPos[i] = Vector2(X4, Y1 - (RightYSpace * (i - 6 - 1)));
	}

	const int clearMin = (int)m_clearSec / 60.f;
	const int clearHour = (clearMin / 60.f) >= 1.f ? (int)(clearMin / 60.f) : 0;
	const int sec = max(0, (int)m_clearSec - (clearMin * 60.0));

	//TIME
	m_texts[TO_INT(FONT_TYPE::TIME)] = L"Time";
	m_texts[TO_INT(FONT_TYPE::TIME) + 1] = std::to_wstring(clearMin) + L":" + std::to_wstring(sec);
	//std::wstring wstTimeStr = L"Time";
	//std::wstring wstTimeVal = std::to_wstring(clearMin) + L":" + std::to_wstring(sec) + L"\n";
	//POW
	m_texts[TO_INT(FONT_TYPE::POW)] = L"POW ";
	m_texts[TO_INT(FONT_TYPE::POW) + 1] = std::to_wstring(m_displayRoboCount);
	//CORPSE
	m_texts[TO_INT(FONT_TYPE::CORPSE)] = L"CRPS ";
	m_texts[TO_INT(FONT_TYPE::CORPSE) + 1] = std::to_wstring(m_corpseCount);

	//DEF
	m_texts[TO_INT(FONT_TYPE::DEF)] = L"DEFAULT";
	m_texts[TO_INT(FONT_TYPE::DEF) + 1] = std::to_wstring((int)m_attachmentPercentageData.defaultAttachment) + L"%";
	//MELEE
	m_texts[TO_INT(FONT_TYPE::MELEE)] = L"MELEE";
	m_texts[TO_INT(FONT_TYPE::MELEE) + 1] = std::to_wstring((int)m_attachmentPercentageData.meleeAttachment) + L"%";
	//REMOTE
	m_texts[TO_INT(FONT_TYPE::REMOTE)] = L"REMOTE";
	m_texts[TO_INT(FONT_TYPE::REMOTE) + 1] = std::to_wstring((int)m_attachmentPercentageData.remoteAttachment) + L"%";
	//BOMB
	m_texts[TO_INT(FONT_TYPE::BOMB)] = L"BOMB";
	m_texts[TO_INT(FONT_TYPE::BOMB) + 1] = std::to_wstring((int)m_attachmentPercentageData.bombAttachment) + L"%";

	for (int i = 0; i < 6; i++) {
		m_fonts[i].Begin();
		m_fonts[i].Draw(m_texts[i].c_str(), m_fontsPos[i], m_FontColor, m_fontRot, m_leftFontSca);
		m_fonts->End();
	}

	for (int i = 6; i < m_NUM_FONTS;  i++) {
		m_fonts[i].Begin();
		m_fonts[i].Draw(m_texts[i].c_str(), m_fontsPos[i], m_FontColor, m_fontRot, m_rightFontSca);
		m_fonts->End();
	}

}
