#pragma once

#include "GBuffer.h"


namespace UER
{
	class PreRender
	{
	public:
		PreRender() {};
		~PreRender() {};

		void Init();
		void Render(RenderContext& rc);

		GBuffer& GetGBuffer()
		{
			return m_gBuffer;
		}
	private:
		GBuffer m_gBuffer;
	};
}