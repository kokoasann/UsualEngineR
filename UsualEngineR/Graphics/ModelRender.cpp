#include "PreCompile.h"
#include "ModelRender.h"



namespace UER
{
	void ModelRender::Release()
	{
		
	}
	void ModelRender::OnDestroy()
	{

	}
	void ModelRender::Init(const ModelInitData& initdata)
	{
		m_model.Init(initdata);
	}
	void ModelRender::InitAnimation(const std::vector<CAnimationClipPtr> &anim, int animNum)
	{
		m_animation.Init(*m_model.GetSkelton(), anim);
	}
	void ModelRender::Awake()
	{
	}
	void ModelRender::Update()
	{
		float dtime = gameTime()->GetDeltaTime();
		m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		m_animation.Progress(dtime);
	}
	void ModelRender::Render()
	{
		auto& rc = g_graphicsEngine->GetRenderContext();
		m_model.Draw(rc);
	}
}