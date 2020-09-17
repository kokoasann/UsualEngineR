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

		m_isRender = true;
	}
	void ModelRender::InitAnimation(const std::vector<CAnimationClipPtr> &anim, int animNum)
	{
		auto ske = m_model.GetSkelton();
		m_animation.Init(*m_model.GetSkelton(), anim);
		ske->SetPlayFlag(true);
	}
	void ModelRender::Awake()
	{
	}
	void ModelRender::Update()
	{
		float dtime = gameTime()->GetDeltaTime();
		m_animation.Progress(dtime);
		m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		
		m_model.GetSkelton()->Update(m_model.GetWorldMatrix());
	}
	void ModelRender::Render()
	{
		if (!m_isRender)
			return;

		auto& rc = g_graphicsEngine->GetRenderContext();
		m_model.Draw(rc);
	}
}