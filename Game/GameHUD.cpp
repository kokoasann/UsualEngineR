#include "stdafx.h"
#include "GameHUD.h"


GameHUD::GameHUD()
{

}

GameHUD::~GameHUD()
{

}



void GameHUD::Release()
{

}

void GameHUD::OnDestroy()
{
	Release();
}


void GameHUD::Awake()
{
	DeleteGO(m_spriteRender);
}

bool GameHUD::Start()
{
	m_spriteRender = NewGO<SpriteRender>(0);

	SpriteInitData sd;
	sd.m_ddsFilePath[0] = "Assets/Image/sample.dds";
	sd.m_width = 50;
	sd.m_height = 50;
	m_spriteRender->Init(sd);
	return true;
}


void GameHUD::PreUpdate()
{

}

void GameHUD::Update()
{

}

void GameHUD::PostUpdate()
{

}


void GameHUD::Render()
{

}

void GameHUD::PostRender()
{

}
