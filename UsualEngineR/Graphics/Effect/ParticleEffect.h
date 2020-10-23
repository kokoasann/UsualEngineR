#pragma once



namespace UER
{
	class ParticleEffect
	{
	public:
		ParticleEffect() {}
		~ParticleEffect() {}

		virtual void Release() = 0;
		//virtual void Update() = 0;
		virtual void Draw(
			RenderContext& rc,
			const Vector3& pos,
			const Vector3& sca,
			const Quaternion& rot,
			const Vector4& mulColor,
			const Matrix& view,
			const Matrix& projection
		) = 0;
	private:

	};



	struct SParticleData
	{
		Matrix mWorld;
		Vector4 mulColor;
	};
	struct SParticleInstanceData
	{
		Vector3 pos;
		Vector3 sca;
		Quaternion rot;
		Vector4 mulColor;
	};
	struct ParticleDataEX
	{
		SParticleInstanceData particleData;
		float lifeTime;
	};
#define PLANE_PARTICLE_GENERATE_ARGS_CONST PlaneParticleEffect* pThis, float deltaTime
#define PLANE_PARTICLE_GENERATE_ARGS(pThis,deltaTime) PLANE_PARTICLE_GENERATE_ARGS_CONST

#define PLANE_PARTICLE_UPDATE_ARGS_CONST ParticleDataEX& data, float deltaTime, void* extendData
#define PLANE_PARTICLE_UPDATE_ARGS(data,deltaTime,extendData) PLANE_PARTICLE_UPDATE_ARGS_CONST

	class PlaneParticleEffect;
	using PlaneParticleGenerateFunc = std::function<void(PLANE_PARTICLE_GENERATE_ARGS_CONST)>;
	using PlaneParticleUpdateFunc = std::function<void(PLANE_PARTICLE_UPDATE_ARGS_CONST)>;




	struct PlaneParticleUpdater
	{
		PlaneParticleUpdater(PlaneParticleGenerateFunc gene, PlaneParticleUpdateFunc update):
			m_geneFunc(std::move(gene)),m_updateFunc(std::move(update))
		{	
		}
		PlaneParticleGenerateFunc m_geneFunc;
		PlaneParticleUpdateFunc m_updateFunc;
		//float m_timer = 0;
	};
	
	struct PlaneParticleEffectInitData
	{
		const wchar_t* m_ddsFilePath = { nullptr };		//DDSファイルのファイルパス。
		UINT m_width = 0;										//スプライトの幅。
		UINT m_height = 0;										//スプライトの高さ。
		bool m_isDepthTest = true;
		bool m_isBillboard = true;
		int m_extendDataSize = 0;
		//PlaneParticleGenerateFunc m_generateFunc;
		//PlaneParticleUpdateFunc m_updateFunc;
		PlaneParticleUpdater* m_updater = nullptr;
	};

	class PlaneParticleEffect final:public ParticleEffect
	{
	public:
		void Release();
		void Init(const PlaneParticleEffectInitData& ini);

		void Update(float deltaTime);

		/// <summary>
		/// 描画。PrePostRenderで読んでやってね!
		/// </summary>
		/// <param name="rc"></param>
		/// <param name="pos"></param>
		/// <param name="sca"></param>
		/// <param name="rot"></param>
		/// <param name="mulColor"></param>
		/// <param name="view"></param>
		/// <param name="projection"></param>
		void Draw(
			RenderContext& rc,
			const Vector3& pos,
			const Vector3& sca,
			const Quaternion& rot,
			const Vector4& mulColor,
			const Matrix& view,
			const Matrix& projection
		);

		template<class T>
		void AddParticle(const Vector3& pos, const Vector3& sca, const Quaternion& rot, const Vector4& mulColor, float lifeTime, T* extendData = nullptr)
		{
			if (m_numInstance >= MAX_INSTANCES_NUM)
				return;
			int len = m_particleDatasEX.size();
			m_particleDatasEX.resize(len + 1);
			m_particleDatasEX[len].particleData.pos = pos;
			m_particleDatasEX[len].particleData.sca = sca;
			m_particleDatasEX[len].particleData.rot = rot;
			m_particleDatasEX[len].particleData.mulColor = mulColor;
			m_particleDatasEX[len].lifeTime = lifeTime;

			if (m_extendDataSize > 0)
			{
				if (sizeof(T) != m_extendDataSize || extendData == nullptr)
					std::abort();

				if (m_numInstance >= m_extendDatas.size())
				{
					void* p = malloc(m_extendDataSize);
					memcpy(p, extendData, m_extendDataSize);
					m_extendDatas.push_back(p);
				}
				else
				{
					memcpy(m_extendDatas[m_numInstance], extendData, m_extendDataSize);
				}
			}

			m_numInstance++;
		}


		/*struct SParticleData
		{
			Matrix mWorld;
			Vector4 mulColor;
		};*/
	private:
		struct SConstBuffData
		{
			SConstBuffData(const Matrix& mVP, const Vector4& mulcolor) :
				mvp(mVP), mulColor(mulcolor)
			{
			}
			Matrix mvp;
			Vector4 mulColor;
		};
		
		
	private:
		static const UINT MAX_INSTANCES_NUM = 2048U;
		IndexBuffer m_indexBuff;
		VertexBuffer m_vertBuff;
		Texture m_texture;
		float m_width = 0;
		float m_height = 0;

		RootSignature m_rootSign;
		DescriptorHeap m_descHeap;
		PipelineState m_pipState;
		ConstantBuffer m_constBuffer;

		//SConstBuffData m_constBufferData;
		
		StructuredBuffer m_structuredBuff;
		//std::vector<SParticleData> m_particleDatas;
		//std::vector<float> m_particleTimes;
		std::vector<ParticleDataEX> m_particleDatasEX;
		std::vector<void*> m_extendDatas;
		int m_extendDataSize = 0;

		int m_numInstance=0;

		Shader m_vs;
		Shader m_ps;

		//PlaneParticleUpdater* m_updater = nullptr;
		PlaneParticleGenerateFunc m_generateFunc;
		PlaneParticleUpdateFunc m_updateFunc;

		bool m_isBillboard = true;
	};





	/*class ModelParticleEffect final:public ParticleEffect
	{
	public:
		void Release();
		void Draw(RenderContext& rc);
	private:

	};*/

	class PlaneParticleEffectRender final:public GameObject
	{
	public:
		void Release() override
		{
			m_effect.Release();
		}
		void OnDestroy() override
		{
			Release();
		}

		void Init(const PlaneParticleEffectInitData& pid);

		void Update() override;


		void PrePostRender() override;



		void SetPos(const Vector3& pos)
		{
			m_pos = pos;
		}
		void SetSca(const Vector3& sca)
		{
			m_sca = sca;
		}
		void SetRot(const Quaternion& rot)
		{
			m_rot = rot;
		}
		void SetMulColor(const Vector4& col)
		{
			m_mulColor = col;
		}

		const Vector3& GetPos() const
		{
			return m_pos;
		}
		const Vector3& GetSca() const
		{
			return m_sca;
		}
		const Quaternion& GetRot() const
		{
			return m_rot;
		}
		const Vector4& GetMulColor() const
		{
			return m_mulColor;
		}
	private:
		PlaneParticleEffect m_effect;
		
		Vector3 m_pos = g_vec3Zero;
		Vector3 m_sca = g_vec3One;
		Quaternion m_rot = g_quatIdentity;
		Vector4 m_mulColor = {1,1,1,1};
	};
}