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
			if (m_camMode != ECameraMode::Rotate)
			{
				
				mSprite.Update(mPosition, qrot, mScale, mPivot);
			}
			else
			{
				auto pro = g_camera3D->GetProjectionMatrix();
				pro.m[3][3] = 1.f;
				pro.m[3][2] = 0.f;
				pro.m[2][3] = 0.f;
				pro.Inverse();
				auto s = mScale;
				pro.Apply(s);

				auto p = mPosition;
				g_camera3D->GetViewProjectionMatrix().Apply(p);
				
				
				mSprite.Update(mPosition, qrot, mScale, mPivot);
			}
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

		switch (m_camMode)
		{
		case ECameraMode::Usual:
			mSprite.Draw(rc, g_camera2D->GetViewMatrix(), g_camera2D->GetProjectionMatrix());
			break;
		case ECameraMode::View:
			mSprite.Draw(rc, g_camera3D->GetViewMatrix(), g_camera3D->GetProjectionMatrix());
			break;
		case ECameraMode::Rotate:
		{
			auto vmat = g_camera3D->GetViewMatrix();
			auto promat = g_camera3D->GetProjectionMatrix();
			//promat.m[3][3] = 1;
			//promat.m[3][2] = 0;
			//promat.m[2][3] = 0;

			auto pos = vmat.GetTransrate();
			auto rot = vmat.GetRotate();
			auto sca = vmat.GetScale();
			Matrix smat,pmat,rmat;
			pmat.MakeTranslation(pos);
			smat.MakeScaling(sca);
			rmat.MakeRotationFromQuaternion(rot);
			//smat.Inverse();
			mSprite.Draw(rc, g_camera3D->GetViewMatrix(), promat);
		}
			break;
		}
	}
	
}