#include "stdafx.h"
#include "GameHUD.h"
#include "Player/Player.h"
#include "Enemy/IEnemy.h"

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
	DeleteGO(m_spPlayerEndurance);
	DeleteGO(m_spPlayerBoost);
}

bool GameHUD::Start()
{


	SpriteInitData sd;
	//Player HP
	m_spPlayerHP = NewGO<SpriteRender>(0);
	sd.m_ddsFilePath[0] = "Assets/Image/hp.dds";
	sd.m_height = m_flSpHpHeight;
	sd.m_width = m_flSpHpWidth;
	m_spPlayerHP->Init(sd);
	m_spPlayerHP->SetPos(m_hpPos);
	m_spPlayerHP->SetSca(m_playerHpScale);
	m_spPlayerHP->SetPivot(m_SPRITE_PIVOT);

	//Player Endurance
	m_spPlayerEndurance = NewGO<SpriteRender>(0);
	sd.m_ddsFilePath[0] = "Assets/Image/hp.dds";
	sd.m_height = m_flSpEnduranceHeight;
	sd.m_width = m_flSpEnduranceWidth;
	m_spPlayerEndurance->Init(sd);
	m_spPlayerEndurance->SetPos(m_endurancePos);
	m_spPlayerEndurance->SetSca(m_playerEnduranceScale);
	m_spPlayerEndurance->SetPivot(m_SPRITE_PIVOT);

	//Player Boost
	m_spPlayerBoost = NewGO<SpriteRender>(0);
	sd.m_ddsFilePath[0] = "Assets/Image/hp.dds";
	sd.m_height = m_flSpBoostHeight;
	sd.m_width = m_flSpBoostWidth;
	m_spPlayerBoost->Init(sd);
	m_spPlayerBoost->SetPos(m_boostPos);
	m_spPlayerBoost->SetSca(m_playerBoostScale);
	m_spPlayerBoost->SetPivot(m_boostSpPivot);

	//Enemy HP
	m_spEnemyHP = NewGO<SpriteRender>(0);
	sd.m_ddsFilePath[0] = "Assets/Image/hp.dds";
	sd.m_height = m_flSpEnemyHPHeight;
	sd.m_width = m_flSpEnemyHPWidth;
	m_spEnemyHP->Init(sd);
	m_spEnemyHP->SetPos(m_enemyHPPos);
	m_spEnemyHP->SetSca(m_enemyHpScale);
	m_spEnemyHP->SetPivot(m_SPRITE_PIVOT);

	//Player Weapon
	weaponsp = NewGO<SpriteRender>(0);
	sd.m_ddsFilePath[0] = "Assets/Image/hp.dds";
	sd.m_height = sph;
	sd.m_width = spw;
	weaponsp->Init(sd);
	weaponsp->SetPos(wpos);
	weaponsp->SetSca(m_playerHpScale);
	weaponsp->SetPivot(m_SPRITE_PIVOT);

	weaponsp2 = NewGO<SpriteRender>(0);
	sd.m_ddsFilePath[0] = "Assets/Image/hp.dds";
	sd.m_height = sph;
	sd.m_width = spw;
	weaponsp2->Init(sd);
	weaponsp2->SetPos(w2pos);
	weaponsp2->SetSca(m_playerHpScale);
	weaponsp2->SetPivot(m_SPRITE_PIVOT);

	return true;
}


void GameHUD::PreUpdate()
{

}

void GameHUD::Update()
{
	//Player HP
	m_playerHpScale.x = m_pPlayer->GetCurrentHP() / m_pPlayer->GetMaxHP();
	m_spPlayerHP->SetSca(m_playerHpScale);

	//Player Endurance
	m_playerEnduranceScale.x = m_pPlayer->GetCurrentEndurance() / m_pPlayer->GetMaxEndurance();
	m_spPlayerEndurance->SetSca(m_playerEnduranceScale);

	//Player Boost
	m_playerBoostScale.y = m_pPlayer->GetCurrentBoost() / m_pPlayer->GetMaxBoost();
	m_spPlayerBoost->SetSca(m_playerBoostScale);

	//Enemy HP
	auto target = m_pPlayer->GetTargetEnemy();
	if (target == nullptr) {
		m_enemyHpScale.x = 0.f;
	}
	else {
		m_enemyHpScale.x = target->GetCurrentHP() / target->GetMaxHP();
	}
	m_spEnemyHP->SetSca(m_enemyHpScale);


	//Player Weapon
	auto nt = m_pPlayer->GetNormalAttackType();
	if (nt == Player::EnAttackType::enSlash) {
		//weaponsp.
	}
	else {

	}

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
