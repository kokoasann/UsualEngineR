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
		m_isInited = true;
	}
	void SpriteRender::Update()
	{
		if (m_isBillBord)
		{
			auto rot = g_camera3D->GetViewMatrixInv();
			rot.SetTranspose({ 0,0,0 });
			Quaternion qrot;
			qrot.SetRotation(rot);
			qrot *= mRotation;
			mSprite.Update(mPosition, qrot, mScale, mPivot);
		}
		else
		{
			mSprite.Update(mPosition, mRotation, mScale, mPivot);
		}

		m_isUpdated = true;
	}
	void SpriteRender::PostRender()
	{
		if (!(m_isInited && m_isUpdated))
			return;

		auto& rc = g_graphicsEngine->GetRenderContext();
		if(!m_isMainCameraView)
			mSprite.Draw(rc,g_camera2D->GetViewMatrix(),g_camera2D->GetProjectionMatrix());
		else
			mSprite.Draw(rc, g_camera3D->GetViewMatrix(), g_camera3D->GetProjectionMatrix());
	}
	
}