#pragma once




namespace UER
{

	class AnchorModel
	{
	public:
		void Release() {}

		void Init(const char* tkmFile,RWStructuredBuffer& vertPosList);

		void Draw(RenderContext& rc,const Matrix& mwvp, bool isInv);

		int GetVertexNum() const
		{
			const auto& view = m_vertBuffer.GetView();
			return view.SizeInBytes / view.StrideInBytes;
		}
	private:
		struct SConstBufferData
		{
			Matrix mwvp;

		};
		TkmFile m_tkm;

		IndexBuffer m_indBuffer;
		IndexBuffer m_indBuffer_inv;
		VertexBuffer m_vertBuffer;
		VertexBuffer m_vertBuffer_inv;

		ConstantBuffer m_constBuffer;

		RootSignature m_rootSign;
		PipelineState m_pipState;
		PipelineState m_pipState_inv;
		DescriptorHeap m_descHeap;

		Shader m_vs;
		Shader m_vs_inv;
		Shader m_ps;
		Shader m_ps_inv;
	};


	class VolumetricEffect
	{
	public:
		void Release();
		void Init(const char* tkmFile);

		void Draw(RenderContext& rc, const Matrix& mworld, const Matrix& mvp, const Vector4 color);
	private:
		struct SConstBufferData
		{
			Matrix mvp;
			Matrix mvp_inv;
		};
		static RenderTarget m_anchorRT;
		static RenderTarget m_anchorRT_inv;
		static RenderTarget m_stencilRT;
		static bool m_isInitRT;

		AnchorModel m_anchorModel;
		Primitive m_prim;
		RWStructuredBuffer m_vertPos;
		ConstantBuffer m_constBuffer;

		RootSignature m_rootSign;
		PipelineState m_pipState;
		DescriptorHeap m_descHeap;

		Shader m_vs;
		Shader m_ps;
	};


	class VolumetricEffectRender :public GameObject
	{
	public:
		virtual void Release() override;
		virtual void OnDestroy() override;

		void Init(const char* tkmFile);

		void Update() override;

		void PrePostRender() override;


		void SetPos(const Vector3& v)
		{
			m_pos = v;
		}
		void SetSca(const Vector3& v)
		{
			m_sca = v;
		}
		void SetRot(const Quaternion& q)
		{
			m_rot = q;
		}
		void SetColor(const Vector4& v)
		{
			m_color = v;
		}
	private:
		VolumetricEffect m_volumeEffect;

		Vector3 m_pos = g_vec3Zero;
		Vector3 m_sca = g_vec3One;
		Quaternion m_rot = g_quatIdentity;

		Vector4 m_color;
	};
}