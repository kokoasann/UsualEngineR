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

		const Vector3& GetCenter() const
		{
			return m_center;
		}
		const Vector3& GetCenterAxis() const
		{
			return m_centerAxis;
		}
		const Vector3& GetCenterBottom() const
		{
			return m_centerBottom;
		}
		const Vector3& GetCenterTop() const
		{
			return m_centerTop;
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

		Vector3 m_center;
		Vector3 m_centerBottom;
		Vector3 m_centerTop;
		Vector3 m_centerAxis;
	};


	class VolumetricEffect
	{
	public:
		struct FogData
		{
			UINT rayCount = 80;          //レイのステップ回数。
			float rayLen = 2.f;           //ステップの距離。

			float decayCenterToXZ = 0.f;        //中心からの減衰。
			float decayCenterToY = 0.f;

			Vector3 color = { 1,1,1 };           //フォグの色。

			float concentration = 0.02;    //濃度。
			float perlinScale = 0.08f;      //パーリンノイズのスケール
			float worleyScale = 0.07f;      //ウォーリーノイズのスケール
			float ratioParlinWorley = 0.4f;    //パーリンノイズとウォーリーノイズの合成の割合(0がPerlin、1がWorley)

			Vector3 offset = g_vec3Zero;              //位置のオフセット
		};


		void Release();
		void Init(const char* tkmFile);

		void Draw(RenderContext& rc, const Matrix& mworld, const Matrix& mvp, const FogData& fogData);


		
	private:
		using PADDING = INT;
		struct SFogData
		{
			UINT rayCount = 80;          //レイのステップ回数。
			float rayLen = 2.f;           //ステップの距離。

			float decayCenterToXZ = 0.f;        //中心からの減衰。
			float decayCenterToY = 0.f;

			Vector3 centerPos = g_vec3Zero;
			PADDING p1;

			Vector3 centerBottom = g_vec3AxisY;
			PADDING p2;

			Vector3 centerTop = g_vec3AxisY;
			PADDING p2_1;

			Vector3 color = {1,1,1};           //フォグの色。
			PADDING p3;

			float concentration = 0.02;    //濃度。
			float perlinScale = 0.08f;      //パーリンノイズのスケール
			float worleyScale = 0.07f;      //ウォーリーノイズのスケール
			float ratioParlinWorley = 0.4f;    //パーリンノイズとウォーリーノイズの合成の割合(0がPerlin、1がWorley)

			Vector3 offset = g_vec3Zero;              //位置のオフセット
			PADDING p4;
			
		};
		struct SConstBufferData
		{
			Matrix mvp;
			Matrix mvp_inv;

			SFogData data;
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

		VolumetricEffect::FogData& GetFogData()
		{
			return m_fogData;
		}
	private:
		VolumetricEffect m_volumeEffect;

		Vector3 m_pos = g_vec3Zero;
		Vector3 m_sca = g_vec3One;
		Quaternion m_rot = g_quatIdentity;

		VolumetricEffect::FogData m_fogData;
	};
}