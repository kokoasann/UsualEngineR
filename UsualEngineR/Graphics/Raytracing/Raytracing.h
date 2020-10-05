#pragma once


namespace UER
{
	namespace raytracing {
		extern 	ID3D12Resource* CreateBuffer(
			ID3D12Device5* pDevice,
			uint64_t size,
			D3D12_RESOURCE_FLAGS flags,
			D3D12_RESOURCE_STATES initState,
			const D3D12_HEAP_PROPERTIES& heapProps
		);

		struct AccelerationStructureBuffers {
			ID3D12Resource* pScratch = nullptr;
			ID3D12Resource* pResult = nullptr;
			ID3D12Resource* pInstanceDesc = nullptr;
		};

		//�q�b�g�O���[�v�B
		enum EHitGroup {
			eHitGroup_Undef = -1,
			eHitGroup_PBRCameraRay,	//PBR�}�e���A���ɃJ�������C���Փ˂���Ƃ��̃q�b�g�O���[�v�B
			eHitGroup_PBRShadowRay,	//PBR�}�e���A���ɃV���h�E���C���Փ˂���Ƃ��̃q�b�g�O���[�v�B
			eHitGroup_Num,			//�q�b�g�O���[�v�̐��B
		};

		const D3D12_HEAP_PROPERTIES kUploadHeapProps =
		{
			D3D12_HEAP_TYPE_UPLOAD,
			D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
			D3D12_MEMORY_POOL_UNKNOWN,
			0,
			0,
		};

		const D3D12_HEAP_PROPERTIES kDefaultHeapProps =
		{
			D3D12_HEAP_TYPE_DEFAULT,
			D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
			D3D12_MEMORY_POOL_UNKNOWN,
			0,
			0
		};

		const int MAX_TRACE_RECURSION_DEPTH = 4;	//���C�g���[�X�̍ċA�Ăяo���̍ő吔�B
														//���ꂪTraceRay���ċA�I�ɌĂяo����ő吔�ł��B
														//
			//���[�J�����[�g�V�O�l�`��
		enum ELocalRootSignature {
			eLocalRootSignature_Empty,				//��̃��[�J�����[�g�V�O�l�`���B
			eLocalRootSignature_Raygen,				//���C�����V�F�[�_�[�p�̃��[�J�����[�g�V�O�l�`���B
			eLocalRootSignature_PBRMaterialHit,		//PBR�}�e���A���Ƀq�b�g�����Ƃ��̃��[�J�����[�g�V�O�l�`���B
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


		//�V�F�[�_�[�f�[�^�\���́B
		struct ShaderData {
			const wchar_t* entryPointName;				//�G���g���[�|�C���g�̖��O�B
			ELocalRootSignature useLocalRootSignature;	//�g�p���郍�[�J�����[�g�V�O�l�`���B
			EShaderCategory category;					//�J�e�S���[�B
			EHitGroup hitgroup;							//�q�b�g�O���[�v�B
														//�J�e�S����eShaderCategory_Miss�AeShaderCategory_RayGenerator�̏ꍇ�A���̃p�����[�^�[�͖�������܂��B
		};
		const ShaderData shaderDatas[] = {
			//entryPointName	useLocalRootSignature				category						hitgroup
			{ L"rayGen",		eLocalRootSignature_Raygen,			eShaderCategory_RayGenerator,	eHitGroup_Undef },
			{ L"miss",			eLocalRootSignature_Empty,			eShaderCategory_Miss,			eHitGroup_Undef },
			{ L"chs",			eLocalRootSignature_PBRMaterialHit,	eShaderCategory_ClosestHit,		eHitGroup_PBRCameraRay },
			{ L"shadowChs",		eLocalRootSignature_PBRMaterialHit,	eShaderCategory_ClosestHit,		eHitGroup_PBRShadowRay },
			{ L"shadowMiss",	eLocalRootSignature_Empty,			eShaderCategory_Miss,			eHitGroup_Undef },
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


		/// <summary>
		/// PBR�}�e���A���Ƀ��C���q�b�g����Ƃ��̃V�F�[�_�[���\�[�X�B
		/// </summary>
		/// <remarks>
		/// ���̗񋓎q�̕��т�t���W�X�^�̔ԍ��ɂȂ�B
		/// </remarks>
		enum class EHitGroup_PBRMaterialHitSRV {
			eTLAS,			//TLAS
			eAlbedoMap,		//�A���x�h�}�b�v�B
			eNormalMap,		//�@���}�b�v�B
			eSpecularMap,	//�X�y�L�����}�b�v�B
			eReflectionMap,	//���t���N�V�����}�b�v�B
			eRefractionMap,	//���܃}�b�v�B
			eVertexBuffer,	//���_�o�b�t�@�B
			eIndexBuffer,	//�C���f�b�N�X�o�b�t�@�B
			eNum,			//SRV�̐��B
		};

		class Instance;
		using InstancePtr = std::unique_ptr< Instance>;
	}//namespace raytracing
}
#include "RaytracingWorld.h"
