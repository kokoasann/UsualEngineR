#pragma once



namespace UER
{
	const int MAX_TRACE_RECURSION_DEPTH = 2;	//レイトレースの再帰呼び出しの最大数。
												//これがTraceRayを再帰的に呼び出せる最大数です。
												//
	//ローカルルートシグネチャ
	enum ELocalRootSignature {
		eLocalRootSignature_Empty,		//空のローカルルートシグネチャ。
		eLocalRootSignature_Raygen,		//レイ生成用のローカルルートシグネチャ。
		eLocalRootSignature_Model,		//モデル用のローカルルートシグネチャ。
	};
	//シェーダー
	enum EShader {
		eShader_Raygeneration,		//カメラレイを生成するシェーダー。
		eShader_Miss,				//カメラレイがどこにもぶつからなかった時に呼ばれるシェーダー。
		eShader_PBRChs,				//もっとも近いポリゴンとカメラレイが交差したときに呼ばれるシェーダー。
		eShader_ShadowChs,			//もっとも近いポリゴンとシャドウレイが交差したときに呼ばれるシェーダー。
		eShader_ShadowMiss,			//シャドウレイがどこにもぶつからなかった時に呼ばれるシェーダー。
		eShader_Num,				//シェーダーの数。
	};
	//シェーダーのカテゴリ。
	enum EShaderCategory {
		eShaderCategory_RayGenerator,	//レイを生成するシェーダー。
		eShaderCategory_Miss,			//ミスシェーダー。
		eShaderCategory_ClosestHit,		//もっとも近いポリゴンとレイが交差したときに呼ばれるシェーダー。
	};

	//ヒットグループ。
	enum EHitGroup {
		eHitGroup_Undef = -1,
		eHitGroup_PBRCameraRay,	//PBRマテリアルにカメラレイが衝突するときのヒットグループ。
		eHitGroup_PBRShadowRay,	//PBRマテリアルにシャドウレイが衝突するときのヒットグループ。
		eHitGroup_Num,			//ヒットグループの数。
	};
	//シェーダーデータ構造体。
	struct ShaderData {
		const wchar_t* entryPointName;				//エントリーポイントの名前。
		ELocalRootSignature useLocalRootSignature;	//使用するローカルルートシグネチャ。
		EShaderCategory category;					//カテゴリー。
		EHitGroup hitgroup;							//ヒットグループ。
													//カテゴリがeShaderCategory_Miss、eShaderCategory_RayGeneratorの場合、このパラメーターは無視されます。
	};
	const ShaderData shaderDatas[] = {
		//entryPointName	useLocalRootSignature			category						hitgroup
		{ L"rayGen",		eLocalRootSignature_Raygen,		eShaderCategory_RayGenerator,	eHitGroup_Undef },
		{ L"miss",			eLocalRootSignature_Empty,		eShaderCategory_Miss,			eHitGroup_Undef },
		{ L"chs",			eLocalRootSignature_Model,		eShaderCategory_ClosestHit,		eHitGroup_PBRCameraRay },
		{ L"shadowChs",		eLocalRootSignature_Empty,		eShaderCategory_ClosestHit,		eHitGroup_PBRShadowRay },
		{ L"shadowMiss",	eLocalRootSignature_Empty,		eShaderCategory_Miss,			eHitGroup_Undef },
	};

	static_assert(ARRAYSIZE(shaderDatas) == eShader_Num, "shaderDatas arraySize is invalid!! shaderDatas arraySize must be equal to eShader_Num");


	struct SHitGroup {
		const wchar_t* name;				//ヒットグループの名前。
		const wchar_t* chsHitShaderName;	//最も近いポリゴンにヒットしたときに呼ばれるシェーダーの名前。
		const wchar_t* anyHitShaderName;	//any shader???
	};
	//ヒットグループの名前の配列。
	const SHitGroup hitGroups[] = {
		{ L"HitGroup",			shaderDatas[eShader_PBRChs].entryPointName,	nullptr },
		{ L"ShadowHitGroup",	shaderDatas[eShader_ShadowChs].entryPointName, nullptr },
	};

	static_assert(ARRAYSIZE(hitGroups) == eHitGroup_Num, "hitGroups arraySize is invalid!! hitGroups arraySize must be equal to eHitGoup_Num");

	const D3D12_HEAP_PROPERTIES kDefaultHeapProps =
	{
		D3D12_HEAP_TYPE_DEFAULT,
		D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
		D3D12_MEMORY_POOL_UNKNOWN,
		0,
		0
	};
	const D3D12_HEAP_PROPERTIES kUploadHeapProps =
	{
		D3D12_HEAP_TYPE_UPLOAD,
		D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
		D3D12_MEMORY_POOL_UNKNOWN,
		0,
		0,
	};

	ID3D12Resource* CreateBuffer(
		ID3D12Device5* pDevice,
		uint64_t size,
		D3D12_RESOURCE_FLAGS flags,
		D3D12_RESOURCE_STATES initState,
		const D3D12_HEAP_PROPERTIES& heapProps)
	{
		D3D12_RESOURCE_DESC bufDesc = {};
		bufDesc.Alignment = 0;
		bufDesc.DepthOrArraySize = 1;
		bufDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		bufDesc.Flags = flags;
		bufDesc.Format = DXGI_FORMAT_UNKNOWN;
		bufDesc.Height = 1;
		bufDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		bufDesc.MipLevels = 1;
		bufDesc.SampleDesc.Count = 1;
		bufDesc.SampleDesc.Quality = 0;
		bufDesc.Width = size;

		ID3D12Resource* pBuffer;
		pDevice->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &bufDesc, initState, nullptr, IID_PPV_ARGS(&pBuffer));
		return pBuffer;
	}
	ID3D12RootSignaturePtr CreateRootSignature(const D3D12_ROOT_SIGNATURE_DESC& desc)
	{
		auto d3dDevice = g_graphicsEngine->GetD3DDevice();
		ID3DBlobPtr pSigBlob;
		ID3DBlobPtr pErrorBlob;
		HRESULT hr = D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &pSigBlob, &pErrorBlob);
		if (FAILED(hr))
		{
			MessageBox(nullptr, L"ルートシグネチャの作成に失敗しました。\n", L"エラー", MB_OK);
			std::abort();
		}
		ID3D12RootSignaturePtr pRootSig;
		d3dDevice->CreateRootSignature(0, pSigBlob->GetBufferPointer(), pSigBlob->GetBufferSize(), IID_PPV_ARGS(&pRootSig));
		return pRootSig;
	}
	struct RootSignatureDesc
	{
		D3D12_ROOT_SIGNATURE_DESC desc = {};
		std::vector<D3D12_DESCRIPTOR_RANGE> range;
		std::vector<D3D12_ROOT_PARAMETER> rootParams;
	};
	//ほげ
	RootSignatureDesc CreateModelSinature()
	{
		RootSignatureDesc desc;
		desc.rootParams.resize(2);
		desc.rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
		desc.rootParams[0].Descriptor.ShaderRegister = 0;
		desc.rootParams[0].Descriptor.RegisterSpace = 0;

		desc.rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
		desc.rootParams[1].Descriptor.ShaderRegister = 1;
		desc.rootParams[1].Descriptor.RegisterSpace = 0;

		desc.desc.NumParameters = 2;
		desc.desc.pParameters = desc.rootParams.data();
		desc.desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE;

		return desc;
	}
	RootSignatureDesc CreateRayGenRootDesc()
	{
		// Create the root-signature
		RootSignatureDesc desc;
		desc.range.resize(2);
		// gOutput
		desc.range[0].BaseShaderRegister = 0;
		desc.range[0].NumDescriptors = 1;
		desc.range[0].RegisterSpace = 0;
		desc.range[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
		desc.range[0].OffsetInDescriptorsFromTableStart = 0;

		// gRtScene
		desc.range[1].BaseShaderRegister = 0;
		desc.range[1].NumDescriptors = 1;
		desc.range[1].RegisterSpace = 0;
		desc.range[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		desc.range[1].OffsetInDescriptorsFromTableStart = 1;

		desc.rootParams.resize(1);
		desc.rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		desc.rootParams[0].DescriptorTable.NumDescriptorRanges = 2;
		desc.rootParams[0].DescriptorTable.pDescriptorRanges = desc.range.data();

		// Create the desc
		desc.desc.NumParameters = 1;
		desc.desc.pParameters = desc.rootParams.data();
		desc.desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE;

		return desc;
	}

	ID3D12DescriptorHeapPtr CreateDescriptorHeap(uint32_t count, D3D12_DESCRIPTOR_HEAP_TYPE type, bool shaderVisible)
	{
		auto d3dDevice = g_graphicsEngine->GetD3DDevice();
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.NumDescriptors = count;
		desc.Type = type;
		desc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

		ID3D12DescriptorHeapPtr pHeap;
		d3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&pHeap));
		return pHeap;
	}

	//サブオブジェクト作成のヘルパー。
	namespace BuildSubObjectHelper {
		/// <summary>
		/// ローカル√シグネチャのサブオブジェクト作成のヘルパー構造体。
		/// </summary>
		struct LocalRootSignatureSubobject {
			LocalRootSignatureSubobject()
			{
			}
			void Init(const D3D12_ROOT_SIGNATURE_DESC& desc)
			{
				pRootSig = CreateRootSignature(desc);
				pInterface = pRootSig;
				subobject.pDesc = &pInterface;
				subobject.Type = D3D12_STATE_SUBOBJECT_TYPE_LOCAL_ROOT_SIGNATURE;
			}
			ID3D12RootSignaturePtr pRootSig;
			ID3D12RootSignature* pInterface = nullptr;
			D3D12_STATE_SUBOBJECT subobject = {};
		};
		/// <summary>
		/// ExportAssociationのサブオブジェクト作成のヘルパー構造体。
		/// </summary>
		struct ExportAssociationSubobject
		{
			void Init(const WCHAR* exportNames[], uint32_t exportCount, const D3D12_STATE_SUBOBJECT* pSubobjectToAssociate)
			{
				association.NumExports = exportCount;
				association.pExports = exportNames;
				association.pSubobjectToAssociate = pSubobjectToAssociate;

				subobject.Type = D3D12_STATE_SUBOBJECT_TYPE_SUBOBJECT_TO_EXPORTS_ASSOCIATION;
				subobject.pDesc = &association;
			}
			D3D12_STATE_SUBOBJECT subobject = {};
			D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION association = {};
		};
		struct ShaderConfigSubobject
		{
			void Init(uint32_t maxAttributeSizeInBytes, uint32_t maxPayloadSizeInBytes)
			{
				shaderConfig.MaxAttributeSizeInBytes = maxAttributeSizeInBytes;
				shaderConfig.MaxPayloadSizeInBytes = maxPayloadSizeInBytes;

				subobject.Type = D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_SHADER_CONFIG;
				subobject.pDesc = &shaderConfig;
			}
			D3D12_RAYTRACING_SHADER_CONFIG shaderConfig = {};
			D3D12_STATE_SUBOBJECT subobject = {};
		};
		/// <summary>
		/// パイプライン設定のサブオブジェクト作成のヘルパー構造体。
		/// </summary>
		struct PipelineConfigSubobject
		{
			PipelineConfigSubobject()
			{
				config.MaxTraceRecursionDepth = MAX_TRACE_RECURSION_DEPTH;

				subobject.Type = D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_PIPELINE_CONFIG;
				subobject.pDesc = &config;
			}

			D3D12_RAYTRACING_PIPELINE_CONFIG config = {};
			D3D12_STATE_SUBOBJECT subobject = {};
		};
		/// <summary>
		/// グローバルルートシグネチャのサブオブジェクト作成のヘルパー構造体。
		/// </summary>
		struct GlobalRootSignatureSubobject
		{
			GlobalRootSignatureSubobject()
			{
				pRootSig = CreateRootSignature({});
				pInterface = pRootSig;
				subobject.pDesc = &pInterface;
				subobject.Type = D3D12_STATE_SUBOBJECT_TYPE_GLOBAL_ROOT_SIGNATURE;
			}
			ID3D12RootSignaturePtr pRootSig;
			ID3D12RootSignature* pInterface = nullptr;
			D3D12_STATE_SUBOBJECT subobject = {};
		};
		/// <summary>
		/// ヒットグループサブオブジェクト
		/// </summary>
		struct HitGroupSubObject
		{
			HitGroupSubObject() {}
			void Init(const SHitGroup& hitgroup)
			{
				desc = {};
				desc.AnyHitShaderImport = hitgroup.anyHitShaderName;
				desc.ClosestHitShaderImport = hitgroup.chsHitShaderName;
				desc.HitGroupExport = hitgroup.name;

				subObject.Type = D3D12_STATE_SUBOBJECT_TYPE_HIT_GROUP;
				subObject.pDesc = &desc;
			}
			D3D12_HIT_GROUP_DESC desc;
			D3D12_STATE_SUBOBJECT subObject;
		};
	};
}