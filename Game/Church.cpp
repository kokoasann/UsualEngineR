#include "stdafx.h"
#include "Church.h"
#include "GameManager.h"
#include "Player/Player.h"
#include "PowaPowa.h"
#include "Fade.h"
#include "GameSceneMenu.h";

Church::Church()
{

}

Church::~Church()
{

}


void Church::Release()
{
	DeleteGO(m_powa);
}

void Church::OnDestroy()
{
	Release();
}


void Church::Awake()
{

}

bool Church::Start()
{

	m_powa = NewGO<PowaPowa>(0);
	return true;
}


void Church::PreUpdate()
{

}

void Church::Update()
{
	auto player = GameManager::GetInstance().GetPlayer();
	if (player == nullptr) return;

	auto len = (player->GetPosition() - m_pos).Length();

	if (len < m_Range) {
		//printf("player is in chruch\n");
		player->SetChruchFlag(true);
	}
	else {
		player->SetChruchFlag(false);
		//printf("player is not in chruch\n");
	}

	 auto& fade = Fade::GetInstance();
	 fade.Update();
	auto menu = GameManager::GetInstance().GetMenu();

	if (fade.IsFaded() and m_doHealPlayer) {
		player->ChargeEndurance(player->GetMaxEndurance());
		player->ChargeBoost(player->GetMaxBoost());
		player->Heal(player->GetMaxHP());
		m_doHealPlayer = false;
		fade.FadeIn();
		m_isFadingIn = true;
	}

	if (m_isFadingIn) {
		if (fade.IsFaded()) {
			m_isFadingIn = false;
			menu->ResumeGame();
		}
	}
}

void Church::PostUpdate()
{

}


void Church::Render()
{

}

void Church::PostRender()
{

}


const bool Church::IsPossibleToHeal() {

	auto player = GameManager::GetInstance().GetPlayer();
	auto len = (player->GetPosition() - m_powa->GetPosition()).Length();

	return len < m_powa->GetRange();

}

void Church::DoHeal() {

	auto menu = GameManager::GetInstance().GetMenu();
	Fade::GetInstance().FadeOut();
	m_doHealPlayer = true;
	menu->PauseGame();

}