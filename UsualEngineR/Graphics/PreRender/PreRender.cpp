#include "PreCompile.h"
#include "PreRender.h"



namespace UER
{
	void PreRender::Init()
	{
		m_gBuffer.Init();
	}
	void PreRender::Render(RenderContext& rc)
	{
		m_gBuffer.Draw(rc);
	}
}