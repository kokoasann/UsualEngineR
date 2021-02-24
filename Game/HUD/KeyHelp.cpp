#include "stdafx.h"
#include "KeyHelp.h"
#include "GameManager.h"
#include "GameSceneMenu.h"

KeyHelp::KeyHelp()
{

}

KeyHelp::~KeyHelp()
{

}



void KeyHelp::Release()
{
}

void KeyHelp::OnDestroy()
{
	Release();
}


void KeyHelp::Awake()
{

}

bool KeyHelp::Start()
{
	return true;
}

void KeyHelp::Init(const Vector3& pos, const std::wstring text) {

	SpriteInitData sd;
	sd.m_ddsFilePath[0] = "Assets/Image/KeyHelpBack.dds";
	sd.m_height = m_flSpHeight;
	sd.m_width = m_flSpWidth;
	m_keyHelpSp.Init(sd);

	m_pos = pos;
	m_text = text;
	m_keyHelpSp.Update(m_pos + m_spPosOffset, Quaternion::Identity, m_spScale);
}

void KeyHelp::Display()
{
	//m_timer = 0.f;
	m_isDisp = true;
	SetActive(true);
}

void KeyHelp::Undisplay()
{
	//m_timer = m_dispTime;
	m_isDisp = false;
}



void KeyHelp::PreUpdate()
{

}

void KeyHelp::Update()
{
	float dtime = gameTime()->GetDeltaTime();

	if (m_isDisp)
	{
		
		m_timer += dtime;
		if (m_timer > m_dispTime)
		{
			m_timer = m_dispTime;
		}
	}
	else
	{
		m_timer -= dtime;
		if (0.f > m_timer)
		{
			m_timer = 0.f;
			SetActive(false);
		}
	}
}

void KeyHelp::PostUpdate()
{

}


void KeyHelp::Render()
{

}

void KeyHelp::PostRender()
{
	if (GameManager::GetInstance().m_menu != nullptr and GameManager::GetInstance().m_menu->IsGamePaused()) return;

	float t = min(1.f,m_timer / m_dispTime);
	//sprite
	auto& rc = g_graphicsEngine->GetRenderContext();
	m_keyHelpSp.Draw(rc, g_camera2D->GetViewMatrix(), g_camera2D->GetProjectionMatrix(), {1,1,1,t});
	//font
	m_color.a = t;
	m_keyHelpText.Begin();
	m_keyHelpText.Draw(m_text.c_str(), Vector2(m_pos.x, m_pos.y) + m_fontPosOffset, m_color, m_rot, m_fontScale, Vector2::Zero);
	m_keyHelpText.End();
}
