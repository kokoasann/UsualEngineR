#pragma once



namespace UER
{
	enum class ERenderSituation : char
	{
		None,
		ShadowMap,
		GBuffer,
		PostRender,
	};

	/// <summary>
	/// 
	/// </summary>
	class RenderingSituation
	{
	private:
		RenderingSituation() {};
		~RenderingSituation() {};
		static RenderingSituation& GetInstance()
		{
			static RenderingSituation instance;
			return instance;
		}
	public:
		static ERenderSituation Get()
		{
			return GetInstance().m_situation;
		}
		static ERenderSituation Set(ERenderSituation stu)
		{
			return GetInstance().m_situation = stu;
		}
	private:
		ERenderSituation m_situation = ERenderSituation::None;
	};
}