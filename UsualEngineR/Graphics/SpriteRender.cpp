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
		mSprite.Draw(rc);
	}
	
}