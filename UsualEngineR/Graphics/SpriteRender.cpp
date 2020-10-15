#include "PreCompile.h"
#include "SpriteRender.h"


namespace UER
{
	void SpriteRender::Release()
	{
	}
	void SpriteRender::OnDestroy()
	{
	}
	void SpriteRender::Init(const SpriteInitData& sid)
	{
		mSprite.Init(sid);
	}
	void SpriteRender::Update()
	{

		mSprite.Update(mPosition, mRotation, mScale, mPivot);
	}
	void SpriteRender::PostRender()
	{
		auto& rc = g_graphicsEngine->GetRenderContext();
		if(!m_isMainCameraView)
			mSprite.Draw(rc,g_camera2D->GetViewMatrix(),g_camera2D->GetProjectionMatrix());
		else
			mSprite.Draw(rc, g_camera3D->GetViewMatrix(), g_camera3D->GetProjectionMatrix());
	}
	
}