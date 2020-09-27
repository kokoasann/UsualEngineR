#pragma once



namespace UER
{
	const int MAX_TRACE_RECURSION_DEPTH = 2;	//���C�g���[�X�̍ċA�Ăяo���̍ő吔�B
												//���ꂪTraceRay���ċA�I�ɌĂяo����ő吔�ł��B
												//
	//���[�J�����[�g�V�O�l�`��
	enum ELocalRootSignature {
		eLocalRootSignature_Empty,		//��̃��[�J�����[�g�V�O�l�`���B
		eLocalRootSignature_Raygen,		//���C�����p�̃��[�J�����[�g�V�O�l�`���B
		eLocalRootSignature_Model,		//���f���p�̃��[�J�����[�g�V�O�l�`���B
	};
	//�V�F�[�_�[
	enum EShader {
		eShader_Raygeneration,		//�J�������C�𐶐�����V�F�[�_�[�B
		eShader_Miss,				//�J�������C���ǂ��ɂ��Ԃ���Ȃ��������ɌĂ΂��V�F�[�_�[�B
		eShader_PBRChs,				//�����Ƃ��߂��|���S���ƃJ�������C�����������Ƃ��ɌĂ΂��V�F�[�_�[�B
		eShader_ShadowChs,			//�����Ƃ��߂��|���S���ƃV���h�E���C�����������Ƃ��ɌĂ΂��V�F�[�_�[�B
		eShader_ShadowMiss,			//�V���h�E���C���ǂ��ɂ��Ԃ���Ȃ��������ɌĂ΂��V�F�[�_�[�B
		eShader_Num,				//�V�F�[�_�[�̐��B
	};
	//�V�F�[�_�[�̃J�e�S���B
	enum EShaderCategory {
		eShaderCategory_RayGenerator,	//���C�𐶐�����V�F�[�_�[�B
		eShaderCategory_Miss,			//�~�X�V�F�[�_�[�B
		eShaderCategory_ClosestHit,		//�����Ƃ��߂��|���S���ƃ��C�����������Ƃ��ɌĂ΂��V�F�[�_�[�B
	};

	//�q�b�g�O���[�v�B
	enum EHitGroup {
		eHitGroup_Undef = -1,
		eHitGroup_PBRCameraRay,	//PBR�}�e���A���ɃJ�������C���Փ˂���Ƃ��̃q�b�g�O���[�v�B
		eHitGroup_PBRShadowRay,	//PBR�}�e���A���ɃV���h�E���C���Փ˂���Ƃ��̃q�b�g�O���[�v�B
		eHitGroup_Num,			//�q�b�g�O���[�v�̐��B
	};
	//�V�F�[�_�[�f�[�^�\���́B
	struct ShaderData {
		const wchar_t* entryPointName;				//�G���g���[�|�C���g�̖��O�B
		ELocalRootSignature useLocalRootSignature;	//�g�p���郍�[�J�����[�g�V�O�l�`���B
		EShaderCategory category;					//�J�e�S���[�B
		EHitGroup hitgroup;							//�q�b�g�O���[�v�B
													//�J�e�S����eShaderCategory_Miss�AeShaderCategory_RayGenerator�̏ꍇ�A���̃p�����[�^�[�͖�������܂��B
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
		const wchar_t* name;				//�q�b�g�O���[�v�̖��O�B
		const wchar_t* chsHitShaderName;	//�ł��߂��|���S���Ƀq�b�g�����Ƃ��ɌĂ΂��V�F�[�_�[�̖��O�B
		const wchar_t* anyHitShaderName;	//any shader???
	};
	//�q�b�g�O���[�v�̖��O�̔z��B
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
			MessageBox(nullptr, L"���[�g�V�O�l�`���̍쐬�Ɏ��s���܂����B\n", L"�G���[", MB_OK);
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
	//�ق�
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

	//�T�u�I�u�W�F�N�g�쐬�̃w���p�[�B
	namespace BuildSubObjectHelper {
		/// <summary>
		/// ���[�J����V�O�l�`���̃T�u�I�u�W�F�N�g�쐬�̃w���p�[�\���́B
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
		/// ExportAssociation�̃T�u�I�u�W�F�N�g�쐬�̃w���p�[�\���́B
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
		/// �p�C�v���C���ݒ�̃T�u�I�u�W�F�N�g�쐬�̃w���p�[�\���́B
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
		/// �O���[�o�����[�g�V�O�l�`���̃T�u�I�u�W�F�N�g�쐬�̃w���p�[�\���́B
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
		/// �q�b�g�O���[�v�T�u�I�u�W�F�N�g
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