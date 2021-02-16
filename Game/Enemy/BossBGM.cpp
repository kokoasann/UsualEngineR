#include "stdafx.h"
#include "BossBGM.h"
#include "GameManager.h"
#include "Game.h"

BossBGM::BossBGM()
{
}

BossBGM::~BossBGM()
{

}



void BossBGM::Release()
{
	DeleteGO(m_bgm);
}

void BossBGM::OnDestroy()
{
	Release();
}


bool BossBGM::Start()
{
	m_bgm = NewGO<CSoundSource>(0);
	m_bgm->Init(L"Assets/sound/Encounter.wav");
	m_bgm->SetVolume(0.0f);

	m_inBattleFlagMap.insert(std::make_pair(IEnemy::EnBossType::Melee, false));
	m_inBattleFlagMap.insert(std::make_pair(IEnemy::EnBossType::Fat, false));
	m_inBattleFlagMap.insert(std::make_pair(IEnemy::EnBossType::Bomb, false));

	return true;
}


void BossBGM::PreUpdate()
{

}

void BossBGM::Update()
{
	
	//if (enemy->GetCurrentState() == enemy->GetState(m_state)) {}
	if (m_inBattleFlagMap[IEnemy::EnBossType::Melee] or m_inBattleFlagMap[IEnemy::EnBossType::Fat] or m_inBattleFlagMap[IEnemy::EnBossType::Bomb]) {
		if (!m_bgm->IsPlaying()) {
			m_bgm->Play(true);
		}
		if (m_bgm->GetVolume() < 1.0f) {
			const float addVolume = 0.1f;
			m_volume += addVolume * gameTime()->GetDeltaTime();
			m_bgm->SetVolume(m_volume);
		}

	}
	else {
		if (m_bgm->GetVolume() <= 0.0f) {
			m_bgm->Stop();
		}
		else{
			const float addVolume = 0.1f;
			m_volume -= addVolume * gameTime()->GetDeltaTime();
			m_bgm->SetVolume(m_volume);
		}
	}
}




