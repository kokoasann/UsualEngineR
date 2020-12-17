#pragma once

#include "TextureCopy.h"

namespace UER
{
	class FocusBlur
	{
	public:
		FocusBlur(){}
		~FocusBlur(){}

		void Init();

		void Render(RenderContext& rc, const Primitive& prim);

		struct BlurData
		{
			int samplingNum = 5;
			float samplingLen = 0.03f;
			Vector2 center = { 0.5,0.5 };
			float decay = 0.2f;
			float time = 0.f;
		};
		BlurData& GetBlurData()
		{
			return m_blurData;
		}
	private:
		
		BlurData m_blurData;
		ConstantBuffer m_constBuff;

		RootSignature m_rootSign;
		DescriptorHeap m_descHeap;
		PipelineState m_pipState;
		Shader m_vs;
		Shader m_ps;
		//Shader m_psCopy;

		RenderTarget m_rt;

		TextureCopy m_texCopy;
	};
}