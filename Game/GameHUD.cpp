#include "stdafx.h"
#include "GameHUD.h"
#include "Player/Player.h"
#include "Enemy/IEnemy.h"
#include "Enemy/EnemyManager.h"
#include "GameManager.h"
#include "HUD/TargetMarker.h"
#include "Camera/GameCamera.h"

GameHUD::GameHUD()
{

}

GameHUD::~GameHUD()
{

}



void GameHUD::Release()
{
	DeleteGO(m_spPlayerHP);
	DeleteGO(m_spPlayerHP_Back);
	DeleteGO(m_spPlayerHP_Deco[0]);
	DeleteGO(m_spPlayerHP_Deco[1]);

	DeleteGO(m_spPlayerEndurance);
	DeleteGO(m_spPlayerEndurance_Back);
	DeleteGO(m_spPlayerEndurance_Deco[0]);
	DeleteGO(m_spPlayerEndurance_Deco[1]);

	DeleteGO(m_spPlayerBoost);
	DeleteGO(m_spPlayerBoost_Back);
	DeleteGO(m_spPlayerBoost_Deco[0]);
	DeleteGO(m_spPlayerBoost_Deco[1]);

	DeleteGO(m_spEnemyHP);
	DeleteGO(m_spEnemyHP_Back);
	DeleteGO(m_spEnemyHP_Deco[0]);
	DeleteGO(m_spEnemyHP_Deco[1]);

	DeleteGO(m_presetBack);
	for (auto sp : m_presetSp)
	{
		DeleteGO(sp);
	}

	DeleteGO(m_targetMarker);
}

void GameHUD::OnDestroy()
{
	Release();
}


void GameHUD::Awake()
{
	m_targetMarker = NewGO<TargetMarker>(0);
}

bool GameHUD::Start()
{

	SpriteInitData sd;
	//Player HP
	{
		m_spPlayerHP = NewGO<SpriteRender>(1);
		sd.m_ddsFilePath[0] = "Assets/Image/white.dds";
		sd.m_height = m_flSpHpHeight;
		sd.m_width = m_flSpHpWidth;
		m_spPlayerHP->Init(sd);
		m_spPlayerHP->SetPos(m_hpPos);
		m_spPlayerHP->SetSca(m_playerHpScale);
		m_spPlayerHP->SetPivot(m_SPRITE_PIVOT);
		m_spPlayerHP->SetMulColor(m_HP_COLOR);
		{
			//Player HP back
			m_spPlayerHP_Back = NewGO<SpriteRender>(0);
			sd.m_ddsFilePath[0] = "Assets/Image/whiteFade_TB.dds";
			sd.m_height = m_flSpHpHeight + 5.f;
			sd.m_width = m_flSpHpWidth + 12.f;
			m_spPlayerHP_Back->Init(sd);
			m_spPlayerHP_Back->SetPos({ m_hpPos.x - 6.f,m_hpPos.y - 2.5f,m_hpPos.z });
			m_spPlayerHP_Back->SetSca(m_playerHpScale);
			m_spPlayerHP_Back->SetPivot(m_SPRITE_PIVOT);
			m_spPlayerHP_Back->SetMulColor(m_BACK_COLOR);
		}
		{
			//Player HP Decoration left
			auto& spPlayerHP_Deco = m_spPlayerHP_Deco[0];
			spPlayerHP_Deco = NewGO<SpriteRender>(1);
			sd.m_ddsFilePath[0] = "Assets/Image/white.dds";
			sd.m_height = m_flSpHpHeight;
			sd.m_width = 5.f;
			spPlayerHP_Deco->Init(sd);
			spPlayerHP_Deco->SetPos({ m_hpPos.x - 7.f,m_hpPos.y,m_hpPos.z });
			spPlayerHP_Deco->SetSca(m_playerHpScale);
			spPlayerHP_Deco->SetPivot(m_SPRITE_PIVOT);
			spPlayerHP_Deco->SetMulColor(m_HP_DECO_COLOR);
		}
		{
			//Player HP Decoration right
			auto& spPlayerHP_Deco = m_spPlayerHP_Deco[1];
			spPlayerHP_Deco = NewGO<SpriteRender>(1);
			sd.m_ddsFilePath[0] = "Assets/Image/white.dds";
			sd.m_height = m_flSpHpHeight;
			sd.m_width = 5.f;
			spPlayerHP_Deco->Init(sd);
			spPlayerHP_Deco->SetPos({ m_hpPos.x + m_flSpHpWidth + 2.f,m_hpPos.y,m_hpPos.z });
			spPlayerHP_Deco->SetSca(m_playerHpScale);
			spPlayerHP_Deco->SetPivot(m_SPRITE_PIVOT);
			spPlayerHP_Deco->SetMulColor(m_HP_DECO_COLOR);
		}
	}

	//Player Endurance
	{
		m_spPlayerEndurance = NewGO<SpriteRender>(1);
		sd.m_ddsFilePath[0] = "Assets/Image/white.dds";
		sd.m_height = m_flSpEnduranceHeight;
		sd.m_width = m_flSpEnduranceWidth;
		m_spPlayerEndurance->Init(sd);
		m_spPlayerEndurance->SetPos(m_endurancePos);
		m_spPlayerEndurance->SetSca(m_playerEnduranceScale);
		m_spPlayerEndurance->SetPivot(m_SPRITE_PIVOT);
		m_spPlayerEndurance->SetMulColor(m_ENDURANCE_COLOR);
		{
			//Player Endurance back
			m_spPlayerEndurance_Back = NewGO<SpriteRender>(0);
			sd.m_ddsFilePath[0] = "Assets/Image/whiteFade_TB.dds";
			sd.m_height = m_flSpEnduranceHeight + 5.f;
			sd.m_width = m_flSpEnduranceWidth + 12.f;
			m_spPlayerEndurance_Back->Init(sd);
			m_spPlayerEndurance_Back->SetPos({ m_endurancePos.x - 6.f,m_endurancePos.y - 2.5f,m_endurancePos.z });
			m_spPlayerEndurance_Back->SetSca(m_playerEnduranceScale);
			m_spPlayerEndurance_Back->SetPivot(m_SPRITE_PIVOT);
			m_spPlayerEndurance_Back->SetMulColor(m_BACK_COLOR);
		}
		{
			//Player Endurance Decoration left
			auto& spPlayerHP_Deco = m_spPlayerEndurance_Deco[0];
			spPlayerHP_Deco = NewGO<SpriteRender>(1);
			sd.m_ddsFilePath[0] = "Assets/Image/white.dds";
			sd.m_height = m_flSpEnduranceHeight;
			sd.m_width = 5.f;
			spPlayerHP_Deco->Init(sd);
			spPlayerHP_Deco->SetPos({ m_endurancePos.x - 7.f,m_endurancePos.y,m_endurancePos.z });
			spPlayerHP_Deco->SetSca(m_playerEnduranceScale);
			spPlayerHP_Deco->SetPivot(m_SPRITE_PIVOT);
			spPlayerHP_Deco->SetMulColor(m_ENDURANCE_DECO_COLOR);
		}
		{
			//Player Endurance Decoration right
			auto& spPlayerHP_Deco = m_spPlayerEndurance_Deco[1];
			spPlayerHP_Deco = NewGO<SpriteRender>(1);
			sd.m_ddsFilePath[0] = "Assets/Image/white.dds";
			sd.m_height = m_flSpEnduranceHeight;
			sd.m_width = 5.f;
			spPlayerHP_Deco->Init(sd);
			spPlayerHP_Deco->SetPos({ m_endurancePos.x + m_flSpEnduranceWidth + 2.f,m_endurancePos.y,m_endurancePos.z });
			spPlayerHP_Deco->SetSca(m_playerEnduranceScale);
			spPlayerHP_Deco->SetPivot(m_SPRITE_PIVOT);
			spPlayerHP_Deco->SetMulColor(m_ENDURANCE_DECO_COLOR);
		}
	}

	//Player Boost
	{
		m_spPlayerBoost = NewGO<SpriteRender>(1);
		sd.m_ddsFilePath[0] = "Assets/Image/white.dds";
		sd.m_height = m_flSpBoostHeight;
		sd.m_width = m_flSpBoostWidth;
		m_spPlayerBoost->Init(sd);
		m_spPlayerBoost->SetPos(m_boostPos);
		m_spPlayerBoost->SetSca(m_playerBoostScale);
		m_spPlayerBoost->SetPivot(m_boostSpPivot);
		m_spPlayerBoost->SetMulColor(m_BOOST_COLOR);
		{
			//Player Boost back
			m_spPlayerBoost_Back = NewGO<SpriteRender>(0);
			sd.m_ddsFilePath[0] = "Assets/Image/whiteFade_TB.dds";
			sd.m_height = m_flSpBoostHeight + 12.f;
			sd.m_width = m_flSpBoostWidth + 5.f;
			m_spPlayerBoost_Back->Init(sd);
			m_spPlayerBoost_Back->SetPos({ m_boostPos.x - 2.5f,m_boostPos.y - 6.f,m_boostPos.z });
			m_spPlayerBoost_Back->SetSca(m_playerBoostScale);
			m_spPlayerBoost_Back->SetPivot(m_SPRITE_PIVOT);
			m_spPlayerBoost_Back->SetMulColor(m_BACK_COLOR);
		}
		{
			//Player Boost Decoration left
			auto& spPlayerHP_Deco = m_spPlayerBoost_Deco[0];
			spPlayerHP_Deco = NewGO<SpriteRender>(1);
			sd.m_ddsFilePath[0] = "Assets/Image/white.dds";
			sd.m_height = 5.f;
			sd.m_width = m_flSpBoostWidth;
			spPlayerHP_Deco->Init(sd);
			spPlayerHP_Deco->SetPos({ m_boostPos.x,m_boostPos.y - 7.f,m_boostPos.z });
			spPlayerHP_Deco->SetSca(m_playerBoostScale);
			spPlayerHP_Deco->SetPivot(m_SPRITE_PIVOT);
			spPlayerHP_Deco->SetMulColor(m_BOOST_DECO_COLOR);
		}
		{
			//Player Boost Decoration right
			auto& spPlayerHP_Deco = m_spPlayerBoost_Deco[1];
			spPlayerHP_Deco = NewGO<SpriteRender>(1);
			sd.m_ddsFilePath[0] = "Assets/Image/white.dds";
			sd.m_height = 5.f;
			sd.m_width = m_flSpBoostWidth;
			spPlayerHP_Deco->Init(sd);
			spPlayerHP_Deco->SetPos({ m_boostPos.x,m_boostPos.y + m_flSpBoostHeight + 2.f,m_boostPos.z });
			spPlayerHP_Deco->SetSca(m_playerBoostScale);
			spPlayerHP_Deco->SetPivot(m_SPRITE_PIVOT);
			spPlayerHP_Deco->SetMulColor(m_BOOST_DECO_COLOR);
		}
	}

	//Enemy HP
	{
		m_spEnemyHP = NewGO<SpriteRender>(1);
		sd.m_ddsFilePath[0] = "Assets/Image/hp.dds";
		sd.m_height = m_flSpEnemyHPHeight;
		sd.m_width = m_flSpEnemyHPWidth;
		m_spEnemyHP->Init(sd);
		m_spEnemyHP->SetPos(m_enemyHPPos);
		m_spEnemyHP->SetSca(m_enemyHpScale);
		m_spEnemyHP->SetPivot(m_SPRITE_PIVOT);

		//Back
		m_spEnemyHP_Back = NewGO<SpriteRender>(0);
		sd.m_ddsFilePath[0] = "Assets/Image/whiteFade_TB.dds";
		sd.m_height = m_flSpEnemyHPHeight+5.f;
		sd.m_width = m_flSpEnemyHPWidth+8.f;
		m_spEnemyHP_Back->Init(sd);
		m_spEnemyHP_Back->SetPos({ m_enemyHPPos.x-4.f,m_enemyHPPos.y-2.5f,m_enemyHPPos.z });
		m_spEnemyHP_Back->SetSca(m_enemyHpScale);
		m_spEnemyHP_Back->SetPivot(m_SPRITE_PIVOT);
		m_spEnemyHP_Back->SetMulColor(m_BACK_COLOR);

		//decoration
		{
			auto& deco = m_spEnemyHP_Deco[0];
			deco = NewGO<SpriteRender>(1);
			sd.m_ddsFilePath[0] = "Assets/Image/white.dds";
			sd.m_height = m_flSpEnemyHPHeight;
			sd.m_width = 5;
			deco->Init(sd);
			deco->SetPos({ m_enemyHPPos.x-7.f,m_enemyHPPos.y,m_enemyHPPos.z });
			deco->SetSca(m_enemyHpScale);
			deco->SetPivot(m_SPRITE_PIVOT);
			deco->SetMulColor(m_HP_DECO_COLOR);
		}
		{
			auto& deco = m_spEnemyHP_Deco[1];
			deco = NewGO<SpriteRender>(1);
			sd.m_ddsFilePath[0] = "Assets/Image/white.dds";
			sd.m_height = m_flSpEnemyHPHeight;
			sd.m_width = 5;
			deco->Init(sd);
			deco->SetPos({ m_enemyHPPos.x + m_flSpEnemyHPWidth +2.f,m_enemyHPPos.y,m_enemyHPPos.z });
			deco->SetSca(m_enemyHpScale);
			deco->SetPivot(m_SPRITE_PIVOT);
			deco->SetMulColor(m_HP_DECO_COLOR);
		}
	}

	//Player Weapon
	m_presetBack = NewGO<SpriteRender>(0);
	sd.m_ddsFilePath[0] = "Assets/Image/attachmentFrame.dds";
	sd.m_height = m_presetBackHeight;
	sd.m_width = m_presetBackWidth;
	m_presetBack->Init(sd);
	m_presetBack->SetPos(m_presetPos);
	m_presetBack->SetSca(Vector3::One);
	m_presetBack->SetPivot(m_PRESET_PIVOT);
	{
		//mulColor ÇÕâºÅB
		{
			m_presetSp[TO_INT(Player::EnAttackPreset::enDefault)] = NewGO<SpriteRender>(0);
			auto presetSp = m_presetSp[TO_INT(Player::EnAttackPreset::enDefault)];
			sd.m_ddsFilePath[0] = "Assets/Image/white.dds";
			sd.m_height = m_presetHeight;
			sd.m_width = m_presetWidth;
			presetSp->Init(sd);
			presetSp->SetPos(m_presetPos + m_presetOffset[TO_INT(Player::EnAttackPreset::enDefault)]);
			presetSp->SetSca(Vector3::One * m_weaponScale);
			presetSp->SetPivot(m_PRESET_PIVOT);
			presetSp->SetMulColor({ 0.5,0.5 ,0.5,1.f });
		}
		{
			m_presetSp[TO_INT(Player::EnAttackPreset::enRemoteAttackPreset)] = NewGO<SpriteRender>(0);
			auto presetSp = m_presetSp[TO_INT(Player::EnAttackPreset::enRemoteAttackPreset)];
			sd.m_ddsFilePath[0] = "Assets/Image/white.dds";
			sd.m_height = m_presetHeight;
			sd.m_width = m_presetWidth;
			presetSp->Init(sd);
			presetSp->SetPos(m_presetPos + m_presetOffset[TO_INT(Player::EnAttackPreset::enRemoteAttackPreset)]);
			presetSp->SetSca(Vector3::One * m_weaponScale);
			presetSp->SetPivot(m_PRESET_PIVOT);
			presetSp->SetMulColor({ 0.8,0. ,0.,1.f });
		}
		{
			m_presetSp[TO_INT(Player::EnAttackPreset::enMeleePreset)] = NewGO<SpriteRender>(0);
			auto presetSp = m_presetSp[TO_INT(Player::EnAttackPreset::enMeleePreset)];
			sd.m_ddsFilePath[0] = "Assets/Image/white.dds";
			sd.m_height = m_presetHeight;
			sd.m_width = m_presetWidth;
			presetSp->Init(sd);
			presetSp->SetPos(m_presetPos + m_presetOffset[TO_INT(Player::EnAttackPreset::enMeleePreset)]);
			presetSp->SetSca(Vector3::One * m_weaponScale);
			presetSp->SetPivot(m_PRESET_PIVOT);
			presetSp->SetMulColor({ 0.,0.8 ,0.,1.f });
		}
		{
			m_presetSp[TO_INT(Player::EnAttackPreset::enExplosivePreset)] = NewGO<SpriteRender>(0);
			auto presetSp = m_presetSp[TO_INT(Player::EnAttackPreset::enExplosivePreset)];
			sd.m_ddsFilePath[0] = "Assets/Image/white.dds";
			sd.m_height = m_presetHeight;
			sd.m_width = m_presetWidth;
			presetSp->Init(sd);
			presetSp->SetPos(m_presetPos + m_presetOffset[TO_INT(Player::EnAttackPreset::enExplosivePreset)]);
			presetSp->SetSca(Vector3::One * m_weaponScale);
			presetSp->SetPivot(m_PRESET_PIVOT);
			presetSp->SetMulColor({ 0.,0. ,0.8,1.f });
		}
	}

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
		m_spEnemyHP_Back->SetActive(false);
		m_spEnemyHP_Deco[0]->SetActive(false);
		m_spEnemyHP_Deco[1]->SetActive(false);
	}
	else {
		m_enemyHpScale.x = target->GetCurrentHP() / target->GetMaxHP();
		m_spEnemyHP_Back->SetActive(true);
		m_spEnemyHP_Deco[0]->SetActive(true);
		m_spEnemyHP_Deco[1]->SetActive(true);
	}
	m_spEnemyHP->SetSca(m_enemyHpScale);
	

	//Player Weapon
	auto preset = player->GetCurrentAttackPreset();

	
	if (m_lateFramePreset != preset)
	{
		if (m_lateFramePreset != Player::EnAttackPreset::enNumPreset)
		{
			m_presetSp[TO_INT(m_lateFramePreset)]->SetPos(m_presetPos + m_presetOffset[TO_INT(m_lateFramePreset)]);
			m_presetSp[TO_INT(m_lateFramePreset)]->SetSca(Vector3::One * m_weaponScale);
		}

		m_presetSp[TO_INT(preset)]->SetPos(m_presetPos);
		m_presetSp[TO_INT(preset)]->SetSca(Vector3::One * m_weaponSelectScale);
		m_lateFramePreset = preset;
	}


	//Marker
	auto gameCamera = GameManager::GetInstance().GetGameCamera();
	m_targetMarker->SetDrawFlag(gameCamera->IsTargettingEnemy());
	auto targetEnemy = EnemyManager::GetEnemyManager().GetTargettingEnemy();
	if (targetEnemy != nullptr) {
		auto pos = targetEnemy->GetPosition();
		pos.y += targetEnemy->GetCharaconHeghit()*0.5f;
		pos.y += targetEnemy->GetCharaconRadius();
		m_targetMarker->SetParentPos(pos);
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
