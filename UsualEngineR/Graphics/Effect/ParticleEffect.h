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
	class PlaneParticleEffect;
	using PlaneParticleGenerateFunc = std::function<void(PlaneParticleEffect* pThis, float deltaTime)>;
	using PlaneParticleUpdateFunc = std::function<void(SParticleData& data,float lifeTime,void* extendData, float deltaTime)>;

#define PLANE_PARTICLE_GENERATE_ARGS(pthis,dtime) PlaneParticleEffect* pthis, float dtime
#define PLANE_PARTICLE_GENERATE_ARGS_CONST PlaneParticleEffect* pThis, float deltaTime

#define PLANE_PARTICLE_UPDATE_ARGS(data,deltaTime) SParticleData& data, float deltaTime
#define PLANE_PARTICLE_UPDATE_ARGS_CONST SParticleData& data, float deltaTime

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

		void Draw(
			RenderContext& rc,
			const Vector3& pos,
			const Vector3& sca,
			const Quaternion& rot,
			const Vector4& mulColor,
			const Matrix& view,
			const Matrix& projection
		);

		void AddParticle(const Matrix& mw, const Vector4& mulColor, float lifeTime);


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
		
		struct ParticleDataEX
		{
			SParticleData particleData;
			float lifeTime;
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
		std::vector<SParticleData> m_particleDatas;
		std::vector<float> m_particleTimes;

		int m_numInstance=0;

		Shader m_vs;
		Shader m_ps;

		//PlaneParticleUpdater* m_updater = nullptr;
		PlaneParticleGenerateFunc m_generateFunc;
		PlaneParticleUpdateFunc m_updateFunc;
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