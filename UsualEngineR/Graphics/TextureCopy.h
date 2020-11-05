#pragma once



namespace UER
{
	class Primitive;

	class TextureCopy
	{
	public:
		enum class BlendMethod :UINT8
		{
			BM_Add,
			BM_Min,
			BM_Mul,
			BM_Div,
			BM_Trans,
		};

		enum class TextureNum :UINT8
		{
			Single,
			Double,
			Triple,
		};

		TextureCopy(){}
		virtual ~TextureCopy()
		{
			Release();
		}
		void Release();

		void Init(Texture* srcTex[3], BlendMethod bm, TextureNum texNum=TextureNum::Single);

		void Render(RenderContext& rc, RenderTarget& rt, const Primitive& prim);
	private:
		DescriptorHeap m_descHeap;
		RootSignature  m_rootSign;
		PipelineState m_pipeState;

		Shader m_vs;
		Shader m_ps;
	};
}