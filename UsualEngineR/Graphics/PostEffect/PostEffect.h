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
	private:
		Primitive m_primitive;

		RenderTarget m_renderTarget;

		Bloom m_bloom;
	};
}