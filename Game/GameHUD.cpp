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


	SpriteInitData sd;
	//Player HP
	m_spPlayerHP = NewGO<SpriteRender>(0);
	sd.m_ddsFilePath[0] = "Assets/Image/sample.dds";
	sd.m_height = m_flSpHpHeight;
	sd.m_width = m_flSpHpWidth;
	m_spPlayerHP->Init(sd);
	m_spPlayerHP->SetPos(m_hpPos);
	m_spPlayerHP->SetSca(m_playerHpScale);
	m_spPlayerHP->SetPivot(m_SPRITE_PIVOT);

	//Player Endurance
	m_spPlayerEndurance = NewGO<SpriteRender>(0);
	sd.m_ddsFilePath[0] = "Assets/Image/sample.dds";
	sd.m_height = m_flSpEnduranceHeight;
	sd.m_width = m_flSpEnduranceWidth;
	m_spPlayerEndurance->Init(sd);
	m_spPlayerEndurance->SetPos(m_endurancePos);
	m_spPlayerEndurance->SetSca(m_playerEnduranceScale);
	m_spPlayerEndurance->SetPivot(m_SPRITE_PIVOT);

	//Player Boost

	//Enemy HP

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
