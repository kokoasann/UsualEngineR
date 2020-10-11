#pragma once

#include "RenderTarget.h"
#include "Primitive.h"
#include "Bloom.h"


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
	private:
		Primitive m_primitive;
		Texture m_backBufferTexture[2];

		RenderContext m_rc4Bloom;

		RenderTarget m_mainRT[2];
		RenderTarget m_renderTarget;

		Bloom m_bloom;
	};
}