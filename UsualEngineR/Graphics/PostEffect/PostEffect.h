#pragma once

#include "RenderTarget.h"
#include "Primitive.h"
#include "Bloom.h"
#include "FocusBlur.h"

namespace UER
{
	class PostEffect
	{
	public:
		PostEffect() {}
		~PostEffect()
		{
			Release();
		}

		void Release();

		void Init();

		void Render();

		Texture* GetCurrentBackBufferTexture();
		

		const Primitive& GetPrimitive() const
		{
			return m_primitive;
		}

		FocusBlur& GetFocusBlur()
		{
			return m_focusBlur;
		}
		void SetUseFocusBlurFrag(bool b)
		{
			m_isUseFocusBlur = b;
		}
		Bloom& GetBloom()
		{
			return m_bloom;
		}
	private:
		Primitive m_primitive;
		Texture m_backBufferTexture[2];

		RenderContext m_rc4Bloom;

		RenderTarget m_mainRT[2];
		RenderTarget m_renderTarget;

		Bloom m_bloom;
		FocusBlur m_focusBlur;

		bool m_isUseFocusBlur = false;
	};
}