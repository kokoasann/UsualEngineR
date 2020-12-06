#pragma once

#include "Model.h"

namespace UER
{
	const static int MAX_SHADOW_MAP = 3;
	/// <summary>
	/// シャドウマップ
	/// </summary>
	class ShadowMap
	{
	public:
		ShadowMap();
		~ShadowMap();
		
		void Init(float w, float h);

		void Update();

		void Send2GPU();

		void Render();

		void AddShadowCaster(SkinModel* m)
		{
			m_shadowCaster.push_back(m);
			
		}
	private:
		struct SShadowCB
		{
			Matrix mLVP[MAX_SHADOW_MAP] = { Matrix::Identity };
			Vector4 texoffset[MAX_SHADOW_MAP];
			Vector4 pixSize[MAX_SHADOW_MAP];
			Vector4 depthoffset;
			float ligNear[MAX_SHADOW_MAP];
			float ligFar[MAX_SHADOW_MAP];
		};
		Vector3 m_lightDirection = Vector3::Down;				//ライトの方向
		float m_near = 0.1f;
		float m_far = 100000.f;
		float m_lightHeight = 1000.f;
		RenderTarget m_shadowMapRT[MAX_SHADOW_MAP];
		ConstantBuffer m_shadowCB;
		std::vector<SkinModel*> m_shadowCaster;
		Matrix m_mLVP[MAX_SHADOW_MAP] = {Matrix::Identity};
		SShadowCB m_shadowCBEntity;
		D3D12_VIEWPORT m_viewPort;
	};
}