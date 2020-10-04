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
	private:
		static const int MAX_WEIGHT = 8;
		struct BlurData
		{
			float weight[MAX_WEIGHT] = {0.f};
		};

		float m_power = 1.0f;

		RenderTarget m_rtX;
		RenderTarget m_rtY;

		RootSignature m_rootSign;
		Shader m_vsX;
		Shader m_vsY;
		Shader m_ps;
		PipelineState m_pipStateX;
		PipelineState m_pipStateY;

		DescriptorHeap m_descHeap;

	};
}