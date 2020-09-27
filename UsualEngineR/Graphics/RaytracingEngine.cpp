
#include "PreCompile.h"
#include "RaytracingHelper.h"

#include "RaytracingEngine.h"
#include "Material.h"



#define align_to(_alignment, _val) (((_val + _alignment - 1) / _alignment) * _alignment)
using namespace std;
namespace UER
{


	void RaytracingEngine::CreatePipelineState(RenderContext& rc)
	{
		using namespace BuildSubObjectHelper;
		std::array<D3D12_STATE_SUBOBJECT, 14> subobjects;
		uint32_t index = 0;

		//DXILライブラリを作成。
		//レイトレーシング用のシェーダーをロード。
		Shader raytraceShader;
		raytraceShader.LoadRaytracing(L"Assets/shader/sample.fx");

		D3D12_EXPORT_DESC libExport[eShader_Num];
		for( int i = 0; i < eShader_Num; i++ ){
			libExport[i].Name = shaderDatas[i].entryPointName;
			libExport[i].ExportToRename = nullptr;
			libExport[i].Flags = D3D12_EXPORT_FLAG_NONE;
		};

		D3D12_DXIL_LIBRARY_DESC dxLibdesc;
		auto pBlob = raytraceShader.GetCompiledDxcBlob();
		dxLibdesc.DXILLibrary.pShaderBytecode = pBlob->GetBufferPointer();
		dxLibdesc.DXILLibrary.BytecodeLength = pBlob->GetBufferSize();
		dxLibdesc.NumExports = ARRAYSIZE(libExport);
		dxLibdesc.pExports = libExport;

		subobjects[index].Type = D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY;
		subobjects[index].pDesc = &dxLibdesc;

		index++;

		//HitGroupのサブオブジェクトを作成。

		std::vector< HitGroupSubObject> hitGroupSOs;
		hitGroupSOs.resize(eHitGroup_Num);
		for (int i = 0; i < eHitGroup_Num; i++) {
			hitGroupSOs[i].Init( hitGroups[i] );
			subobjects[index++] = hitGroupSOs[i].subObject; // 1 Hit Group
		}
	
		auto BuildAndRegistRootSignatureAndAssSubobjectFunc = [&](
			LocalRootSignatureSubobject& rsSO, ExportAssociationSubobject& ass, ELocalRootSignature eRS, const WCHAR* exportNames[]
		) {
			if (eRS == eLocalRootSignature_Raygen) {
				rsSO.Init(CreateRayGenRootDesc().desc);
			}
			if (eRS == eLocalRootSignature_Model) {
				rsSO.Init(CreateModelSinature().desc);
			}
			if (eRS == eLocalRootSignature_Empty) {
				D3D12_ROOT_SIGNATURE_DESC emptyDesc = {};
				emptyDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE;
				rsSO.Init(emptyDesc);
			}
			subobjects[index] = rsSO.subobject;
			uint32_t rgSOIndex = index++;
		
			int useRootSignature = 0;
			for (auto& shaderData : shaderDatas) {
				if (shaderData.useLocalRootSignature == eRS) {
					//こいつは指定されたルートシグネチャを使う。
					exportNames[useRootSignature] = shaderData.entryPointName;
					useRootSignature++;
				}
			}
			ass.Init(exportNames, useRootSignature, &(subobjects[rgSOIndex]));
			subobjects[index++] = ass.subobject;
		};
		//ルートシグネチャとシェーダーの関連付けを行うサブオブジェクトを作っていく。
		LocalRootSignatureSubobject rayGenSignatureSO, modelSignatureSO, emptySignatureSO;
		ExportAssociationSubobject rayGenAssSO, modelAssSO, emptyAssSO;
		const WCHAR* rayGenExportName[eShader_Num];
		const WCHAR* modelExportName[eShader_Num];
		const WCHAR* emptyExportName[eShader_Num];

		BuildAndRegistRootSignatureAndAssSubobjectFunc(rayGenSignatureSO, rayGenAssSO, eLocalRootSignature_Raygen, rayGenExportName);
		BuildAndRegistRootSignatureAndAssSubobjectFunc(modelSignatureSO, modelAssSO, eLocalRootSignature_Model, modelExportName);
		BuildAndRegistRootSignatureAndAssSubobjectFunc(emptySignatureSO, emptyAssSO, eLocalRootSignature_Empty, emptyExportName);


		// Payloadのサイズと引数の数はとりあえず固定で・・・。後で検討。
		ShaderConfigSubobject shaderConfig;
		shaderConfig.Init(sizeof(float) * 2, sizeof(float) * 10);
		subobjects[index] = shaderConfig.subobject; // 

		uint32_t shaderConfigIndex = index++; 
		ExportAssociationSubobject configAssociationSO;
		const WCHAR* entryPointNames[eShader_Num];
		for (int i = 0; i < eShader_Num; i++) {
			entryPointNames[i] = shaderDatas[i].entryPointName;
		}
		configAssociationSO.Init(entryPointNames, eShader_Num, &subobjects[shaderConfigIndex]);
		subobjects[index++] = configAssociationSO.subobject;

		// パイプライン設定のサブオブジェクトを作成。
		PipelineConfigSubobject config;
		subobjects[index++] = config.subobject; 

		 // グローバルルートシグネチャのサブオブジェクトを作成。
		GlobalRootSignatureSubobject root;
		m_emptyRootSignature = root.pRootSig;
		subobjects[index++] = root.subobject; 
	
		// Create the state
		D3D12_STATE_OBJECT_DESC desc;
		desc.NumSubobjects = index;
		desc.pSubobjects = subobjects.data();
		desc.Type = D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE;
		auto d3dDevice = g_graphicsEngine->GetD3DDevice();
		auto hr = d3dDevice->CreateStateObject(&desc, IID_PPV_ARGS(&m_pipelineState));
		if (FAILED(hr) ){
			MessageBox(nullptr, L"パイプラインステートの作成に失敗しました。\n", L"エラー", MB_OK);
			std::abort();
		}
	}
	void RaytracingEngine::CreateShaderResources()
	{
		auto d3dDevice = g_graphicsEngine->GetD3DDevice();
		// Create the output resource. The dimensions and format should match the swap-chain
		D3D12_RESOURCE_DESC resDesc = {};
		resDesc.DepthOrArraySize = 1;
		resDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		resDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // The backbuffer is actually DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, but sRGB formats can't be used with UAVs. We will convert to sRGB ourselves in the shader
		resDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
		resDesc.Height = g_graphicsEngine->GetFrameBufferHeight();
		resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		resDesc.MipLevels = 1;
		resDesc.SampleDesc.Count = 1;
		resDesc.Width = g_graphicsEngine->GetFrameBufferWidth();
		d3dDevice->CreateCommittedResource(&kDefaultHeapProps, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_COPY_SOURCE, nullptr, IID_PPV_ARGS(&m_outputResource)); // Starting as copy-source to simplify onFrameRender()

		// Create an SRV/UAV descriptor heap. Need 2 entries - 1 SRV for the scene and 1 UAV for the output
		m_srvUavHeap = CreateDescriptorHeap(2, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, true);

		// Create the UAV. Based on the root signature we created it should be the first entry
		D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
		uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
		d3dDevice->CreateUnorderedAccessView(m_outputResource, nullptr, &uavDesc, m_srvUavHeap->GetCPUDescriptorHandleForHeapStart());

		// Create the TLAS SRV right after the UAV. Note that we are using a different SRV desc here
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.RaytracingAccelerationStructure.Location = m_topLevelASBuffers.pResult->GetGPUVirtualAddress();
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = m_srvUavHeap->GetCPUDescriptorHandleForHeapStart();
		srvHandle.ptr += d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		d3dDevice->CreateShaderResourceView(nullptr, &srvDesc, srvHandle);
	}

	void RaytracingEngine::CreateShaderTable(RenderContext& rc)
	{
		//各シェーダーの数を調査。
		m_numRayGenShader = 0;
		m_numMissShader = 0;
		m_numHitShader = 0;
		for (auto& shaderData : shaderDatas) {
			if (shaderData.category == eShaderCategory_RayGenerator) m_numRayGenShader++;
			if (shaderData.category == eShaderCategory_Miss) m_numMissShader++;
			if (shaderData.category == eShaderCategory_ClosestHit) m_numHitShader++;
		}
		// Calculate the size and create the buffer
		m_shaderTableEntrySize = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
		m_shaderTableEntrySize += sizeof(D3D12_GPU_DESCRIPTOR_HANDLE) * 2; // The hit shader
		m_shaderTableEntrySize = align_to(D3D12_RAYTRACING_SHADER_RECORD_BYTE_ALIGNMENT, m_shaderTableEntrySize);
		uint32_t shaderTableSize = m_shaderTableEntrySize * (m_numRayGenShader + m_numMissShader + (m_numHitShader * m_instances.size()));

		auto d3dDevice = g_graphicsEngine->GetD3DDevice();
		//シェーダーテーブル用のバッファを作成。
		m_shaderTable = CreateBuffer(d3dDevice, shaderTableSize, D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ, kUploadHeapProps);

		//バッファをシステムメモリにマップする。
		uint8_t* pData;
		m_shaderTable->Map(0, nullptr, (void**)&pData);

		CComPtr< ID3D12StateObjectProperties > pRtsoProps;
		m_pipelineState->QueryInterface(&pRtsoProps);
	
		uint8_t* pCurret = pData;

		//シェーダーテーブルにシェーダーを登録する関数。
		auto RegistShaderTblFunc = [&](const ShaderData& shaderData, EShaderCategory registCategory, RaytracingInstance* instance){
			if (shaderData.category == registCategory) {
				//まずシェーダーIDを設定する。
				void* pShaderId = nullptr;
				if (registCategory == eShaderCategory_ClosestHit) {
					pShaderId = pRtsoProps->GetShaderIdentifier(hitGroups[shaderData.hitgroup].name);
				}
				else {
					pShaderId = pRtsoProps->GetShaderIdentifier(shaderData.entryPointName);
				}
				memcpy(pCurret, pShaderId, D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
				if (shaderData.useLocalRootSignature == eLocalRootSignature_Raygen) {
					//デフォルトのルートシグネチャを使用する場合、シェーダーIDの後ろにディスクリプタヒープへのアドレスを設定する。
					uint64_t heapStart = m_srvUavHeap->GetGPUDescriptorHandleForHeapStart().ptr;
					*(uint64_t*)(pCurret + D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES) = heapStart;
				}
				if (shaderData.useLocalRootSignature == eLocalRootSignature_Model) {
					uint8_t* pDst = pCurret + D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;

					*(uint64_t*)(pDst) = m_topLevelASBuffers.pResult->GetGPUVirtualAddress();	//シーン。
					pDst += sizeof(D3D12_GPU_DESCRIPTOR_HANDLE);
					/*D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
					cpuHandle.ptr = (SIZE_T)pDst;
					instance->m_albedoTexture->RegistShaderResourceView(cpuHandle, 0);*/
				}
				//次。
				pCurret += m_shaderTableEntrySize;
			}
		};
		// レイジェネレーションシェーダーをテーブルに登録していく。
		for (auto& shader : shaderDatas) {
			RegistShaderTblFunc(shader, eShaderCategory_RayGenerator, nullptr);
		}
		// 続いてミスシェーダー。
		for (auto& shader : shaderDatas) {
			RegistShaderTblFunc(shader, eShaderCategory_Miss, nullptr);
		}
		//最後にヒットシェーダー。ヒットシェーダーはヒットシェーダーの数　×　インスタンスの数だけ登録する。
		for (auto& instance : m_instances) {
			for (auto& shader : shaderDatas) {
				RegistShaderTblFunc(shader, eShaderCategory_ClosestHit, &instance);
			};
		}

		//Unmap
		m_shaderTable->Unmap(0, nullptr);
	}
	void RaytracingEngine::Dispatch(RenderContext& rc)
	{
		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Transition.pResource = m_outputResource;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_SOURCE;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
		rc.ResourceBarrier(barrier);

		D3D12_DISPATCH_RAYS_DESC raytraceDesc = {};
		raytraceDesc.Width = g_graphicsEngine->GetFrameBufferWidth();
		raytraceDesc.Height = g_graphicsEngine->GetFrameBufferHeight();
		raytraceDesc.Depth = 1;

	
		// レイ生成シェーダーのシェーダーテーブルの開始アドレスとサイズを設定。
		raytraceDesc.RayGenerationShaderRecord.StartAddress = m_shaderTable->GetGPUVirtualAddress();
		raytraceDesc.RayGenerationShaderRecord.SizeInBytes = m_shaderTableEntrySize;

		// ミスシェーダーのシェーダーテーブルの開始アドレスとサイズを設定。
		size_t missOffset = m_numRayGenShader * m_shaderTableEntrySize;
		raytraceDesc.MissShaderTable.StartAddress = m_shaderTable->GetGPUVirtualAddress() + missOffset;
		raytraceDesc.MissShaderTable.StrideInBytes = m_shaderTableEntrySize;
		raytraceDesc.MissShaderTable.SizeInBytes = m_shaderTableEntrySize * m_numMissShader;

		// ヒットグループシェーダーの開始アドレスとサイズを設定。
		size_t hitOffset = (m_numRayGenShader+m_numMissShader) * m_shaderTableEntrySize;
		raytraceDesc.HitGroupTable.StartAddress = m_shaderTable->GetGPUVirtualAddress() + hitOffset;
		raytraceDesc.HitGroupTable.StrideInBytes = m_shaderTableEntrySize;
		raytraceDesc.HitGroupTable.SizeInBytes = m_shaderTableEntrySize * m_numHitShader * m_instances.size();

		// グローバルルートシグネチャを設定。
		rc.SetComputeRootSignature(m_emptyRootSignature);

		// Dispatch
		rc.SetDescriptorHeap(m_srvUavHeap);
		rc.SetPipelineState1(m_pipelineState);
		rc.DispatchRays(raytraceDesc);

		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Transition.pResource = m_outputResource;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_SOURCE;
		rc.ResourceBarrier(barrier);

		//レイトレの結果をフレームバッファに書き戻す。
		g_graphicsEngine->CopyToFrameBuffer(rc, m_outputResource);

	}
	#include <time.h>



		void RaytracingEngine::RegistGeometry(Model& model)
		{
			RaytracingInstance instance;
			model.QueryMeshs([&](const SMesh& mesh) {
			
				for (int i = 0; i < mesh.m_materials.size(); i++ ){
					const auto& vertexBufferView = mesh.m_vertexBuffer.GetView();
					const auto& indexBufferView = mesh.m_indexBufferArray[i]->GetView();
					D3D12_RAYTRACING_GEOMETRY_DESC desc;
					memset(&desc, 0, sizeof(desc));
					desc.Type = D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES;
					desc.Triangles.Transform3x4 = 0;
					desc.Triangles.VertexBuffer.StartAddress = vertexBufferView.BufferLocation;
					desc.Triangles.VertexBuffer.StrideInBytes = vertexBufferView.StrideInBytes;
					desc.Triangles.VertexCount = vertexBufferView.SizeInBytes / vertexBufferView.StrideInBytes;
					desc.Triangles.VertexFormat = DXGI_FORMAT_R32G32B32_FLOAT;
					desc.Flags = D3D12_RAYTRACING_GEOMETRY_FLAG_OPAQUE;
					desc.Triangles.IndexBuffer = indexBufferView.BufferLocation;
					desc.Triangles.IndexCount = mesh.m_indexBufferArray[i]->GetCount();
					desc.Triangles.IndexFormat = indexBufferView.Format;
					instance.geometoryDesc = desc;			
					instance.m_albedoTexture = &mesh.m_materials[i]->GetAlbedoMap();
					m_instances.push_back(std::move(instance));
				}
			});
		}
		void RaytracingEngine::BuildBottomLevelAS(RenderContext& rc)
		{
			for (auto& instance : m_instances) {
				D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS inputs = {};
				inputs.DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY;
				inputs.Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_NONE;
				inputs.NumDescs = 1;
				inputs.pGeometryDescs = &instance.geometoryDesc;
				inputs.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL;
	
				auto d3dDevice = g_graphicsEngine->GetD3DDevice();
				D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO info;
				d3dDevice->GetRaytracingAccelerationStructurePrebuildInfo(&inputs, &info);
	
				AccelerationStructureBuffers asbuffer;
				asbuffer.pScratch = CreateBuffer(
					d3dDevice,
					info.ScratchDataSizeInBytes,
					D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
					D3D12_RESOURCE_STATE_COMMON,
					kDefaultHeapProps);
	
				asbuffer.pResult = CreateBuffer(
					d3dDevice,
					info.ResultDataMaxSizeInBytes,
					D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
					D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE,
					kDefaultHeapProps);
	
				// Create the bottom-level AS
				D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC asDesc = {};
				asDesc.Inputs = inputs;
				asDesc.DestAccelerationStructureData = asbuffer.pResult->GetGPUVirtualAddress();
				asDesc.ScratchAccelerationStructureData = asbuffer.pScratch->GetGPUVirtualAddress();
	
				rc.BuildRaytracingAccelerationStructure(asDesc);
	
				//レイトレーシングアクセラレーション構造のビルド完了待ちのバリアを入れる。
				D3D12_RESOURCE_BARRIER uavBarrier = {};
				uavBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
				uavBarrier.UAV.pResource = asbuffer.pResult;
				rc.ResourceBarrier(uavBarrier);
	
				m_bottomLevelASBuffers.push_back(std::move(asbuffer));
			}
		}
		void RaytracingEngine::BuildTopLevelAS(RenderContext& rc, bool update)
		{
			uint64_t tlasSize;
			auto d3dDevice = g_graphicsEngine->GetD3DDevice();
			//todo インスタンスの数は1で固定している。
			int numInstance = m_instances.size();
			D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS inputs = {};
			inputs.DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY;
			inputs.Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_ALLOW_UPDATE;
			inputs.NumDescs = numInstance;
			inputs.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL;
	
			D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO info;
			d3dDevice->GetRaytracingAccelerationStructurePrebuildInfo(&inputs, &info);
	
			if (update) {
				//更新？
			}
			else {
				//新規？
				m_topLevelASBuffers.pScratch = CreateBuffer(d3dDevice, info.ScratchDataSizeInBytes, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, kDefaultHeapProps);
				m_topLevelASBuffers.pResult = CreateBuffer(d3dDevice, info.ResultDataMaxSizeInBytes, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE, kDefaultHeapProps);
				m_topLevelASBuffers.pInstanceDesc = CreateBuffer(
					d3dDevice, 
					sizeof(D3D12_RAYTRACING_INSTANCE_DESC) * numInstance,
					D3D12_RESOURCE_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ, 
					kUploadHeapProps
				);
				tlasSize = info.ResultDataMaxSizeInBytes;
			}
	
			//Map the instance desc buffer
			D3D12_RAYTRACING_INSTANCE_DESC* instanceDescs;
			m_topLevelASBuffers.pInstanceDesc->Map(0, nullptr, (void**)&instanceDescs);
			ZeroMemory(instanceDescs, sizeof(D3D12_RAYTRACING_INSTANCE_DESC) * numInstance);
		
			Matrix mRot;
			mRot.MakeRotationX(Math::PI * -0.5f);
			mRot.Transpose();
	
			for (int i = 0; i < numInstance; i++) {
				instanceDescs[i].InstanceID = i;
				//instanceDescs[i].InstanceContributionToHitGroupIndex = eHitGroup_PBRCameraRay;
				instanceDescs[i].InstanceContributionToHitGroupIndex = (int)eHitGroup_Num * i + eHitGroup_PBRCameraRay;
				instanceDescs[i].Flags = D3D12_RAYTRACING_INSTANCE_FLAG_NONE;
				instanceDescs[i].AccelerationStructure = m_bottomLevelASBuffers[i].pResult->GetGPUVirtualAddress();
				memcpy(instanceDescs[i].Transform, &mRot, sizeof(instanceDescs[i].Transform));
				instanceDescs[i].InstanceMask = 0xFF;
			}
	
			m_topLevelASBuffers.pInstanceDesc->Unmap(0, nullptr);
	
			//TopLevelASを作成。
			D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC asDesc = {};
			asDesc.Inputs = inputs;
			asDesc.Inputs.InstanceDescs = m_topLevelASBuffers.pInstanceDesc->GetGPUVirtualAddress();
			asDesc.DestAccelerationStructureData = m_topLevelASBuffers.pResult->GetGPUVirtualAddress();
			asDesc.ScratchAccelerationStructureData = m_topLevelASBuffers.pScratch->GetGPUVirtualAddress();
		
			if (update)
			{
				asDesc.Inputs.Flags |= D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PERFORM_UPDATE;
				asDesc.SourceAccelerationStructureData = m_topLevelASBuffers.pResult->GetGPUVirtualAddress();
			}
			rc.BuildRaytracingAccelerationStructure(asDesc);
	
			//レイトレーシングアクセラレーション構造のビルド完了待ちのバリアを入れる。
			D3D12_RESOURCE_BARRIER uavBarrier = {};
			uavBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
			uavBarrier.UAV.pResource = m_topLevelASBuffers.pResult;
			rc.ResourceBarrier(uavBarrier);
		}
		void RaytracingEngine::CommitRegistGeometry(RenderContext& rc)
		{
			BuildBottomLevelAS(rc);
			BuildTopLevelAS(rc, false);
	
			CreatePipelineState(rc);	//パイプラインステートを作成。
			CreateShaderResources();	//シェーダーリソースを作成。
			CreateShaderTable(rc);		//シェーダーテーブルを作成。
		}

}
