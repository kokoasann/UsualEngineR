#pragma once



namespace UER
{
	enum class EGBufferKind : INT8
	{
		Diffuse,
		Depth,
		Normal,
		Specular,
		Tangent,

		//Move,

		NumKind,
	};

	class GBuffer
	{
	public:

		void Init();

		void BegineRender(RenderContext& rc);
		void EndRender(RenderContext& rc);
		void Draw(RenderContext& rc);
	private:
		RenderTarget m_gBuffer[TO_INT(EGBufferKind::NumKind)];

		std::vector<GameObject*> m_renderObjects;
	};
}