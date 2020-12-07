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

		m_isInit = true;
	}
	void ModelRender::InitAnimation(const std::map<int,CAnimationClipPtr> &anim, int animNum)
	{
		auto ske = m_model.GetSkelton();
		m_animation.Init(*m_model.GetSkelton(), anim);
		ske->SetPlayFlag(true);
		
	}
	void ModelRender::Awake()
	{
		//SetThreadingFunction(GameObject::tf_Update);
	}

	void ModelRender::Update()
	{
		if (!m_isInit)
		{
			return;
		}
		m_isRender = true;

		float dtime = gameTime()->GetDeltaTime();
		m_animation.Progress(dtime*m_animPlaySpeed);
		
		m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		m_model.UpdateIK();
		m_model.UpdateSkeleton();
		
		//m_model.UpdateSkeleton();

		if(m_isShadowCaster)
			g_graphicsEngine->GetShadowMap().AddShadowCaster(&m_model);


	}
	void ModelRender::Render()
	{
		if (!m_isRender || !m_isInit)
			return;

		auto& rc = g_graphicsEngine->GetRenderContext();
		m_model.Draw(rc,m_mulColor);
	}
}