#pragma once



namespace UER
{
	class FocusBlur
	{
	public:
		FocusBlur();
		~FocusBlur();

		void Init();

		void Draw(RenderContext& rc);
	private:
		struct BlurData
		{
			int samplingNum;
			float samplingLen;
			Vector2 center = {0.5,0.5};
			float decay = 0.2f;
		};
		BlurData m_blurData;
		ConstantBuffer m_constBuff;
		

		RootSignature m_rootSign;
		DescriptorHeap m_descHeap;
		PipelineState m_pipState;
		Shader m_vs;
		Shader m_ps;
		//Shader m_psCopy;

		RenderTarget m_rt;
	};
}