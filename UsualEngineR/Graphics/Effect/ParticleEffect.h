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
		virtual void Draw(RenderContext& rc) = 0;
	private:

	};
	struct SParticleData
	{
		Matrix mWorld;
		Vector4 mulColor;
	};

	typedef std::function<void* (int& instanceNum, SParticleData* particleDatas, float* particleTimers, float deltaTime)> PlaneParticleGenerateFunc;
	typedef std::function<void* (SParticleData* datas,int num)> PlaneParticleUpdateFunc;

	struct PlaneParticleUpdater
	{
		PlaneParticleGenerateFunc m_geneFunc;
		PlaneParticleUpdateFunc m_updateFunc;
		float m_timer = 0;
	};
	
	struct PlaneParticleEffectInitData
	{
		const wchar_t* m_ddsFilePath = { nullptr };		//DDSファイルのファイルパス。
		UINT m_width = 0;										//スプライトの幅。
		UINT m_height = 0;										//スプライトの高さ。
		bool m_isDepthTest = true;
		PlaneParticleGenerateFunc m_generateFunc;
		PlaneParticleUpdateFunc m_updateFunc;

	};

	class PlaneParticleEffect final:public ParticleEffect
	{
	public:
		void Release();
		void Init(const PlaneParticleEffectInitData& ini);

		void Update(float deltaTime,const Vector3& pos,const Quaternion& rot,const Vector3& sca);

		void Draw(RenderContext& rc);

		struct SParticleData
		{
			Matrix mWorld;
			Vector4 mulColor;
		};
	private:
		struct SConstBuffData
		{
			Matrix mvp;
			Vector4 mulColor;
			Vector4 ScreenParam;
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
		
		StructuredBuffer m_structuredBuff;
		std::array<SParticleData, MAX_INSTANCES_NUM> m_particleDatas;
		std::array<float, MAX_INSTANCES_NUM> m_particleTimes;
		int m_numInstance=0;

		Shader m_vs;
		Shader m_ps;

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
}