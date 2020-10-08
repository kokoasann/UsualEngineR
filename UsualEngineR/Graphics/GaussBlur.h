#pragma once



namespace UER
{
	class GaussBlur
	{
	public:
		GaussBlur(){}
		virtual ~GaussBlur() 
		{
			Release();
		}

		void Release();

		void Init(int w, int h, RenderTarget* inTex, int inW, int inH, RenderTarget*& outTex);

		void Render(RenderContext& rc, Primitive prim);

		Texture* GetOutputTexture()
		{
			return &m_rtY.GetRenderTargetTexture();
		}
	private:
		static const int MAX_WEIGHT = 8;
		struct BlurData
		{
			Vector4 offset;
			float weight[MAX_WEIGHT] = {0.f};
			float rttexRatio = 1.f;
		};
		BlurData m_blurData;

		float m_power = 1.0f;

		RenderTarget m_rtX;
		RenderTarget m_rtY;

		RootSignature m_rootSign;
		Shader m_vsX;
		Shader m_vsY;
		Shader m_ps;
		PipelineState m_pipStateX;
		PipelineState m_pipStateY;

		DescriptorHeap m_descHeapX;
		DescriptorHeap m_descHeapY;

		Texture m_inputTex;
		ConstantBuffer m_constBuffer;

		int m_width = 0;
		int m_height = 0;

		int m_inWidth = 0;
		int m_inHeight = 0;
		
	};
}