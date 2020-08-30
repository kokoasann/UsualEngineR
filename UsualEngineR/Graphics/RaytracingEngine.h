#pragma once


namespace UER
{
	
	class Model;
	
	using ID3D12ResourcePtr = CComPtr<ID3D12Resource>;
	using ID3D12RootSignaturePtr = CComPtr<ID3D12RootSignature>;
	using ID3DBlobPtr = CComPtr<ID3DBlob>;
	using ID3D12StateObjectPtr = CComPtr<ID3D12StateObject>;
	using ID3D12DescriptorHeapPtr = CComPtr<ID3D12DescriptorHeap>;
	
	class RaytracingEngine
	{
	public:
		/// <summary>
		/// レイトレーシングをディスパッチ。
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		void Dispatch(RenderContext& rc);
		/// <summary>
		/// ジオメトリを登録。
		/// </summary>
		/// <param name="model">モデル</param>
		void RegistGeometry(Model& model);
		/// <summary>
		/// ジオメトリの登録を確定。
		/// </summary>
		void CommitRegistGeometry(RenderContext& rc);
	private:
		/// <summary>
		/// パイプラインステートを作成。
		/// </summary>
		/// <param name="rc"></param>
		void CreatePipelineState(RenderContext& rc);
		/// <summary>
		/// シェーダーテーブルを作成。
		/// </summary>
		/// <param name="rc"></param>
		void CreateShaderTable(RenderContext& rc);
		/// <summary>
		/// BottomLevelASを構築。
		/// </summary>
		/// <param name="rc"></param>
		void BuildBottomLevelAS(RenderContext& rc);
		/// <summary>
		/// TopLevelASを構築。
		/// </summary>
		void BuildTopLevelAS(RenderContext& rc, bool update);
		/// <summary>
		/// シェーダーリソースを作成。
		/// </summary>
		void CreateShaderResources();
	private:
		struct AccelerationStructureBuffers {
			ID3D12Resource* pScratch = nullptr;
			ID3D12Resource* pResult = nullptr;
			ID3D12Resource* pInstanceDesc = nullptr;
		};
		
		/// <summary>
		/// レイトレのインスタンスデータ。
		/// </summary>
		struct RaytracingInstance {
			D3D12_RAYTRACING_GEOMETRY_DESC geometoryDesc;	//ジオメトリ情報。
			Texture* m_albedoTexture = nullptr;		//アルベドテクスチャ。
		};
		std::vector<RaytracingInstance> m_instances;	//レイトレーシングエンジンに登録するジオメトリ情報の配列。
		std::vector< AccelerationStructureBuffers> m_bottomLevelASBuffers;
		AccelerationStructureBuffers m_topLevelASBuffers;
		ID3D12StateObjectPtr m_pipelineState;							//パイプラインステート
		ID3D12RootSignaturePtr m_emptyRootSignature;
		ID3D12DescriptorHeapPtr m_srvUavHeap;
		ID3D12ResourcePtr m_outputResource;								//レイトレースの結果の出力先。
		ID3D12ResourcePtr m_shaderTable;								//シェーダーテーブル。
		uint32_t m_shaderTableEntrySize = 0;
		int m_numRayGenShader = 0;
		int m_numMissShader = 0;
		int m_numHitShader = 0;
	};

}
