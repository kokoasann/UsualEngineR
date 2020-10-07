#pragma once

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

		Texture* GetCurrentBackBufferTexture()
		{
			return &m_backBufferTexture[g_graphicsEngine->GetBackBufferIndex()];
		}
	private:
		Primitive m_primitive;
		Texture m_backBufferTexture[2];

		RenderTarget m_renderTarget;

		Bloom m_bloom;
	};
}