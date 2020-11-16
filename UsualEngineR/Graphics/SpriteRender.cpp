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
			mSprite.Draw(rc,g_camera2D->GetViewMatrix(),g_camera2D->GetProjectionMatrix(),m_mulColor);
		else
			mSprite.Draw(rc, g_camera3D->GetViewMatrix(), g_camera3D->GetProjectionMatrix(), m_mulColor);

		switch (m_camMode)
		{
		case ECameraMode::Usual:
			mSprite.Draw(rc, g_camera2D->GetViewMatrix(), g_camera2D->GetProjectionMatrix(), m_mulColor);
			break;
		case ECameraMode::View:
			mSprite.Draw(rc, g_camera3D->GetViewMatrix(), g_camera3D->GetProjectionMatrix(), m_mulColor);
			break;
#if 0
		case ECameraMode::Rotate:
		{
			auto vmat = g_camera3D->GetViewMatrix();
			auto promat = g_camera3D->GetProjectionMatrix();
			//promat.m[3][3] = 1;
			//promat.m[3][2] = 0;
			//promat.m[2][3] = 0;
			
			{
				Matrix v, p;
				v.MakeLookAt({ 0,0,0 }, { 0,0,1 }, { 0,1,0 });
				p.MakeProjectionMatrix(Math::DegToRad(60), FRAME_BUFFER_W / FRAME_BUFFER_H, 5, 50);
				Matrix world;
				Matrix mTrans, mRot, mSca;
				Matrix mvp, mwv, mwvp;
				mvp = v * p;

				mTrans.MakeTranslation({0,50,50});
				mRot.MakeRotationFromQuaternion(mRotation);
				mSca.MakeScaling(mScale);
				world = mSca;
				world = world * mRot;
				world = world * mTrans;

				mwv = world * v;
				mwvp = mwv * p;

				printf("");
			}


			auto pos = vmat.GetTransrate();
			auto rot = vmat.GetRotate();
			auto sca = vmat.GetScale();
			Matrix smat,pmat,rmat;
			pmat.MakeTranslation(pos);
			smat.MakeScaling(sca);
			rmat.MakeRotationFromQuaternion(rot);
			//smat.Inverse();
			mSprite.Draw(rc, g_camera3D->GetViewMatrix(), promat,m_mulColor);
		}
			break;
#endif
		}
	}
	
}