#pragma once



namespace UER
{
	class Primitive;

	class TextureCopy
	{
	public:
		enum class BlendMethod :INT8
		{
			BM_Add,
			BM_Min,
			BM_Mul,
			BM_Div,
			BM_Trans,
		};

		TextureCopy(){}
		virtual ~TextureCopy()
		{
			Release();
		}
		void Release();

		void Init(Texture& srcTex, BlendMethod bm);

		void Render(RenderContext& rc, RenderTarget& rt, const Primitive& prim);
	private:
		DescriptorHeap m_descHeap;
		RootSignature  m_rootSign;
		PipelineState m_pipeState;

		Shader m_vs;
		Shader m_ps;
	};
}