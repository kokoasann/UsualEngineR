#include "stdafx.h"
#include "BossBGM.h"
#include "Enemy/IEnemy.h"

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
	//m_state = TO_INT(IEnemy::EnState::enBattleState);
	return true;
}


void BossBGM::PreUpdate()
{

}

void BossBGM::Update()
{
	if (m_IEnemy->GetCurrentState() == m_IEnemy->GetState(m_state)) {
		if (!m_bgm->IsPlaying()) {
			m_bgm->Play(true);
		}
		if (m_bgm->GetVolume() < 1.0f) {
			const float addVolume = 0.1f;
			m_volume += addVolume * gameTime()->GetDeltaTime();
			m_bgm->SetVolume(m_volume);
		}
	}
	
	if (m_IEnemy->GetCurrentState() == m_IEnemy->GetState(TO_INT(IEnemy::EnState::enIdleState))) {
		if (m_bgm->GetVolume() <= 0.0f) {
			m_bgm->Stop();
		}
		else if (m_bgm->GetVolume() < 1.0f) {
			const float addVolume = 0.1f;
			m_volume -= addVolume * gameTime()->GetDeltaTime();
			m_bgm->SetVolume(m_volume);
		}
	}
}




