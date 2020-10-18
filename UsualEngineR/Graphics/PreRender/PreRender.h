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

		void LightingRender(RenderContext& rc);

		GBuffer& GetGBuffer()
		{
			return m_gBuffer;
		}
	private:
		GBuffer m_gBuffer;

		struct DeferredCB
		{
			Matrix mVPI;
			Vector4 camPos;
			Vector4 camDir;
		};
		ConstantBuffer m_deferredCB;

		RootSignature m_rootSign;
		PipelineState m_pipState;
		DescriptorHeap m_descHeap;
		Shader m_vs;
		Shader m_ps;
	};
}