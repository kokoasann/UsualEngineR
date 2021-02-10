#include "stdafx.h"
#include "Title.h"
#include "Game.h"
#include "Fade.h"
#include "OpeningScene.h"

Title::Title()
{

}

Title::~Title()
{

}



void Title::Release()
{
	DeleteGO(m_backSprite);
}

void Title::OnDestroy()
{
	Release();
}


void Title::Awake()
{

}

bool Title::Start()
{
	Fade::GetInstance().FadeIn();

	SpriteInitData sd;
	sd.m_ddsFilePath[0] = "Assets/Image/title.dds";
	sd.m_width = 1280;
	sd.m_height = 720;
	m_backSprite = NewGO<SpriteRender>(0);
	m_backSprite->Init(sd);
	m_backSprite->SetPos(m_backSpPos);

	return true;
}


void Title::PreUpdate()
{
}

void Title::Update()
{
	if (g_pad[0]->IsTrigger(enButtonA) and !m_isTrigeredStart) {
		Fade::GetInstance().FadeOut();
		m_isTrigeredStart = true;
	}

	if (Fade::GetInstance().IsFaded() and m_isTrigeredStart) {

		//NewGO<Game>(0);
		NewGO<OpeningScene>(0);
		auto go = reinterpret_cast<GameObject*>(this);
		DeleteGO(go);
	}

}

void Title::PostUpdate()
{
	Fade::GetInstance().Update();
}


void Title::Render()
{

}

void Title::PostRender()
{

}
