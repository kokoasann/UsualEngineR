#include "stdafx.h"
#include "GameHUD.h"
#include "Player/Player.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "GameManager.h"

GameHUD::GameHUD()
{

}

GameHUD::~GameHUD()
{

}



void GameHUD::Release()
{
	DeleteGO(m_spPlayerHP);
	DeleteGO(m_spPlayerEndurance);
	DeleteGO(m_spPlayerBoost);

	DeleteGO(m_spEnemyHP);
	DeleteGO(m_presetSp);
}

void GameHUD::OnDestroy()
{
	Release();
}


void GameHUD::Awake()
{

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
	m_presetSp = NewGO<SpriteRender>(0);
	sd.m_ddsFilePath[0] = "Assets/Image/hp.dds";
	sd.m_height = m_presetSpHeight;
	sd.m_width = m_presetSpWidth;
	m_presetSp->Init(sd);
	m_presetSp->SetPos(m_presetSpPos);
	m_presetSp->SetSca(m_playerHpScale);
	m_presetSp->SetPivot(m_SPRITE_PIVOT);

	return true;
}


void GameHUD::PreUpdate()
{

}

void GameHUD::Update()
{

	auto player = GameManager::GetInstance().m_player;
	if (player == nullptr) return;

	//Player HP
	m_playerHpScale.x = player->GetCurrentHP() / player->GetMaxHP();
	m_spPlayerHP->SetSca(m_playerHpScale);

	//Player Endurance
	m_playerEnduranceScale.x = player->GetCurrentEndurance() / player->GetMaxEndurance();
	m_spPlayerEndurance->SetSca(m_playerEnduranceScale);

	//Player Boost
	m_playerBoostScale.y = player->GetCurrentBoost() / player->GetMaxBoost();
	m_spPlayerBoost->SetSca(m_playerBoostScale);

	//Enemy HP
	//auto target = m_pPlayer->GetTargetEnemy();
	auto target = EnemyManager::GetEnemyManager().GetTargettingEnemy();
	if (target == nullptr or !target->IsBoss()) {
		m_enemyHpScale.x = 0.f;
	}
	else {
		m_enemyHpScale.x = target->GetCurrentHP() / target->GetMaxHP();
	}
	m_spEnemyHP->SetSca(m_enemyHpScale);

	//Player Weapon
	auto preset = player->GetCurrentAttackPreset();

	if (preset == Player::EnAttackPreset::enDefault) {
		m_presetSp->SetMulColor({ 0,0,0,1 });
	}

	if (preset == Player::EnAttackPreset::enRemoteAttackPreset) {
		m_presetSp->SetMulColor({ 1,0,0,1 });
	}

	if (preset == Player::EnAttackPreset::enMeleePreset) {
		m_presetSp->SetMulColor({ 0,1,0,1 });
	}

	if (preset == Player::EnAttackPreset::enExposivePreset) {
		m_presetSp->SetMulColor({ 0,0,1,1 });
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
