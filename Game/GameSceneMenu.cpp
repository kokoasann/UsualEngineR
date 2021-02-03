#include "stdafx.h"
#include "GameSceneMenu.h"
#include "GameManager.h"
#include "Game.h"

GameSceneMenu::GameSceneMenu()
{

}

GameSceneMenu::~GameSceneMenu()
{

}



void GameSceneMenu::Release()
{
}

void GameSceneMenu::OnDestroy()
{
	Release();
}


void GameSceneMenu::Awake()
{

}

bool GameSceneMenu::Start()
{
	//sprite
	SpriteInitData sd;
	sd.m_ddsFilePath[0] = "Assets/Image/white.dds";
	sd.m_height = m_flSpHeight;
	sd.m_width = m_flSpWidth;
	m_menuSprite.Init(sd);
	m_menuSprite.Update(m_menuSpritePos, Quaternion::Identity, m_menuSpriteScale);

	m_texts.resize(TO_INT(EnMenuButtonType::NumType));
	m_fonts.resize(TO_INT(EnMenuButtonType::NumType));
	m_texts[TO_INT(EnMenuButtonType::Restart)] = L"‚Í‚¶‚ß‚©‚ç";
	m_texts[TO_INT(EnMenuButtonType::ToTitle)] = L"ƒ^ƒCƒgƒ‹‚Ö";
	m_texts[TO_INT(EnMenuButtonType::Close)] = L"‚Æ‚¶‚é";

	return true;
}


void GameSceneMenu::PreUpdate()
{

}

void GameSceneMenu::Update()
{
}

void GameSceneMenu::PostUpdate()
{
	if (g_pad[0]->IsTrigger(enButtonStart)) {
		if (m_isPaused) {
			m_isMenuActivated = true;
			m_selectedTypeID = TO_INT(EnMenuButtonType::Close);
		}
		else {
			m_isMenuActivated = false;
		}
	}

	if (!m_isMenuActivated) return;

	if (g_pad[0]->IsTrigger(enButtonUp)) {
		m_selectedTypeID = max(0, m_selectedTypeID - 1);
	}
	if (g_pad[0]->IsTrigger(enButtonDown)) {
		m_selectedTypeID = min(TO_INT(EnMenuButtonType::NumType) - 1, m_selectedTypeID + 1);
	}

	if (g_pad[0]->IsTrigger(enButtonA)) {
		const auto& gameManager = GameManager::GetInstance();
		switch (m_selectedTypeID) {
		case TO_INT(EnMenuButtonType::Restart):
			gameManager.m_gameScene->Restart();
			break;
		case TO_INT(EnMenuButtonType::ToTitle):
			gameManager.m_gameScene->ToTitle();
			break;
		case TO_INT(EnMenuButtonType::Close):
			ResumeGame();
			m_isMenuActivated = false;
			break;
		}
	}
}


void GameSceneMenu::Render()
{

}

void GameSceneMenu::PostRender()
{
	if (!m_isMenuActivated) return;

	//sprite
	auto& rc = g_graphicsEngine->GetRenderContext();
	m_menuSprite.Draw(rc, g_camera2D->GetViewMatrix(), g_camera2D->GetProjectionMatrix(), m_spriteMulCol);

	//font
	static const Vector2 pivot = { 0.f, 0.f };
	static const Vector2 fontTopPos = { 0.f, 240.f };
	static const float space = 150.f;
	for (int i = 0; i < TO_INT(EnMenuButtonType::NumType); i++) {
		m_fonts[i].Begin();
		auto pos = fontTopPos;
		pos.y -= i * space;
		
		if (i == m_selectedTypeID) {
			std::wstring text = L"[" + m_texts[i] + L"]";
			pos.x -= ((m_fonts[i].MeasureString(text.c_str()).x / 2.f));
			m_fonts[i].Draw(text.c_str(), pos, m_FontColor, 0, m_FontScale, pivot);
		}
		else {
			pos.x -= (m_fonts[i].MeasureString(m_texts[i].c_str()).x / 2.f);
			m_fonts[i].Draw(m_texts[i].c_str(), pos, m_FontColor, 0, m_FontScale, pivot);
		}

		m_fonts[i].End();
	}
}
