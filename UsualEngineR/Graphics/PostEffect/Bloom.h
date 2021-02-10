#pragma once

#include "RenderTarget.h"
#include "GaussBlur.h"
#include "TextureCopy.h"

#include "LightStreeks.h"
namespace UER
{
	class PostEffect;
	class Bloom
	{
	public:
		Bloom(){}
		~Bloom()
		{
			Release();
		}

		void Release();

		void Init(PostEffect* pe);

		void Render(RenderContext& rc);

		void CombineRender(RenderContext& rc, RenderTarget* rt);

		void SetStreekFrag(bool b)
		{
			m_isAndStreeks = b;
		}
	private:
		PostEffect* m_postEffect = nullptr;

		const static int MAX_GAUSS = 5;
		GaussBlur m_blur[MAX_GAUSS];

		RenderTarget m_luminanceRT;
		RenderTarget m_combineRT;
		
		Shader m_vsBloom;
		Shader m_psLuminance;
		Shader m_psCombine;

		RootSignature m_rootSign;
		PipelineState m_pipeLuminance;
		PipelineState m_pipeCombine;

		DescriptorHeap m_descLuminance;
		DescriptorHeap m_descCombine;

		TextureCopy m_copy;
		TextureCopy m_copy_single;

		LightStreeks m_lightStreeks;

		bool m_isAndStreeks = true;
	};
}