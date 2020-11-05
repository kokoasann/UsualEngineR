#pragma once

#include "TextureCopy.h"



namespace UER
{
	class LightStreeks
	{
	public:
		void Init(float width,float height, Texture* inTex, Texture*& outTex);
		void Draw(RenderContext& rc,const Primitive& prim);

		void SetStreeksDirX(const Vector2& dir)
		{
			m_constBuffDataX.streeksDir = dir;
		}
		void SetStreeksDirY(const Vector2& dir)
		{
			m_constBuffDataY.streeksDir = dir;
		}
		void SetStreeksLenX(float len)
		{
			m_constBuffDataX.streeksLen = len;
		}
		void SetStreeksLenY(float len)
		{
			m_constBuffDataY.streeksLen = len;
		}
		void UpdateWeight(float d);
	private:
		struct SConstBuffData
		{
			float weights[8] = { 0.f };
			Vector2 streeksDir;
			float streeksLen = 20.f;
		};

		RootSignature m_rootSign;
		DescriptorHeap m_descHeapX;
		DescriptorHeap m_descHeapY;
		PipelineState m_pipState;
		PipelineState m_pipStateY;

		Shader m_vs;
		Shader m_ps;

		SConstBuffData m_constBuffDataX;
		SConstBuffData m_constBuffDataY;
		ConstantBuffer m_constBufferX;
		ConstantBuffer m_constBufferY;

		RenderTarget m_renderTargetX;
		RenderTarget m_renderTargetY;

		TextureCopy m_texCopy;
	};
}