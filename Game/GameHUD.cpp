#include "stdafx.h"
#include "GameHUD.h"
#include "Player/Player.h"

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
	DeleteGO(m_spPlayerHP);
}

bool GameHUD::Start()
{

	m_spPlayerHP = NewGO<SpriteRender>(0);

	SpriteInitData sd;
	sd.m_ddsFilePath[0] = "Assets/Image/sample.dds";
	sd.m_height = m_flSpHpHeight;
	sd.m_width = m_flSpHpWidth;
	m_spPlayerHP->Init(sd);
	m_spPlayerHP->SetPos(m_hpPos);

	m_spPlayerHP->SetSca(m_playerHpScale);
	m_spPlayerHP->SetPivot(m_SPRITE_PIVOT);

	return true;
}


void GameHUD::PreUpdate()
{

}

void GameHUD::Update()
{
	m_playerHpScale.x = m_pPlayer->GetCurrentHP() / m_pPlayer->GetMaxHP();
	m_spPlayerHP->SetSca(m_playerHpScale);
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
