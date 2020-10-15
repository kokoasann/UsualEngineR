#include "PreCompile.h"
#include "GBuffer.h"




namespace UER
{
	inline void InitRenderTarget(RenderTarget& rt,DXGI_FORMAT texForm,DXGI_FORMAT dsForm,const Vector4& ccolor, const wchar_t* name)
	{
		rt.Create(FRAME_BUFFER_W, FRAME_BUFFER_H, 1, 1, texForm, dsForm, ccolor.v);
		rt.SetName(name);
	}
	void GBuffer::Init()
	{
		InitRenderTarget(
			m_gBuffer[TO_INT(EGBufferKind::Diffuse)],
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			{ 0.5f,0.5f,0.5f,1 },
			L"GB_Diffuse");

		InitRenderTarget(
			m_gBuffer[TO_INT(EGBufferKind::Depth)],
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_UNKNOWN,
			{ 1.f,1.f,g_camera3D->GetFar()- g_camera3D->GetNear(),1.f },
			L"GB_Depth");

		InitRenderTarget(
			m_gBuffer[TO_INT(EGBufferKind::Normal)],
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_UNKNOWN,
			{ 0.f,1.f,0.f,1 },
			L"GB_Normal");

		InitRenderTarget(
			m_gBuffer[TO_INT(EGBufferKind::Specular)],
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_UNKNOWN,
			{ 0,0,0,1 },
			L"GB_Specular");

		InitRenderTarget(m_gBuffer[TO_INT(EGBufferKind::Tangent)],
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_UNKNOWN,
			{ 1,0,0,1 },
			L"GB_Tangent");
	}
	void GBuffer::BegineRender(RenderContext& rc)
	{
		rc.WaitUntilToPossibleSetRenderTargets(TO_INT(EGBufferKind::NumKind), m_gBuffer);
		rc.SetRenderTargets(TO_UINT(EGBufferKind::NumKind), m_gBuffer);

		for (auto& gb : m_gBuffer)
		{
			gb.Clear(rc);
		}

	}
	void GBuffer::EndRender(RenderContext& rc)
	{
		rc.WaitUntilFinishDrawingToRenderTargets(TO_INT(EGBufferKind::NumKind), m_gBuffer);
	}
	void GBuffer::Draw(RenderContext& rc)
	{
		rc.WaitUntilToPossibleSetRenderTargets(TO_INT(EGBufferKind::NumKind),m_gBuffer);
		rc.SetRenderTargets(TO_UINT(EGBufferKind::NumKind), m_gBuffer);

		for (auto& gb : m_gBuffer)
		{
			gb.Clear(rc);
		}

		for (auto obj : m_renderObjects)
		{
			obj->Render();
		}

		rc.WaitUntilFinishDrawingToRenderTargets(TO_INT(EGBufferKind::NumKind), m_gBuffer);
	}
}