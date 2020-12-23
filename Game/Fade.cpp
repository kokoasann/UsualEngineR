#include "stdafx.h"
#include "Fade.h"


Fade::Fade()
{

}

Fade::~Fade()
{

}



void Fade::Release()
{
	DeleteGO(m_panel);
}

void Fade::OnDestroy()
{

}


void Fade::Awake()
{

}

bool Fade::Start()
{

	m_panel = NewGO<SpriteRender>(10);
	SpriteInitData sd;
	sd.m_ddsFilePath[0] = "Assets/Image/white.dds";
	sd.m_height = m_panelHeight;
	sd.m_width = m_panelWidth;
	m_panel->Init(sd);
	m_panel->SetSca(m_panelScale);
	m_panel->SetMulColor(Vector4::Black);

	return true;
}


void Fade::PreUpdate()
{

}

void Fade::Update()
{
	float delta = gameTime()->GetDeltaTime();

	if (m_state == State::enIdle) {
		return;
	}

	if (m_state == State::enFadeIn) {
		m_alpha = max(0.f, m_alpha - m_fadeSpeed * delta);
		if (m_alpha == 0.f) {
			m_state = State::enIdle;
			m_isFaded = true;
		}
	}

	if (m_state == State::enFadeOut) {
		m_alpha = min(1.f, m_alpha + m_fadeSpeed * delta);
		if (m_alpha == 1.f) {
			m_state = State::enIdle;
			m_isFaded = true;
		}
	}

	m_panel->SetMulColor({ 0,0,0,m_alpha });

}

void Fade::PostUpdate()
{

}


void Fade::Render()
{

}

void Fade::PostRender()
{

}
