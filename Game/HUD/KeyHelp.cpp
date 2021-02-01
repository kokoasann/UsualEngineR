#include "stdafx.h"
#include "KeyHelp.h"


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



void KeyHelp::PreUpdate()
{

}

void KeyHelp::Update()
{

}

void KeyHelp::PostUpdate()
{

}


void KeyHelp::Render()
{

}

void KeyHelp::PostRender()
{

	m_keyHelpText.Begin();
	m_keyHelpText.Draw(m_text.c_str(), Vector2(m_pos.x, m_pos.y) + m_fontPosOffset, m_color, m_rot, m_fontScale, Vector2::Zero);
	m_keyHelpText.End();

	auto& rc = g_graphicsEngine->GetRenderContext();
	m_keyHelpSp.Draw(rc, g_camera2D->GetViewMatrix(), g_camera2D->GetProjectionMatrix(), Vector4::White);
}
