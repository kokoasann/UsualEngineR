#include "PreCompile.h"
#include "RaytracingInstance.h"

#include "RaytracingHelper.h"
#include "Material.h"

namespace UER
{
	RaytracingInstance::~RaytracingInstance()
	{
		Release();
	}
	void RaytracingInstance::Release()
	{
	}
	void RaytracingInstance::Build(Model& model)
	{
		model.QueryMeshs([&](const SMesh& mesh) {

			for (int i = 0; i < mesh.m_materials.size(); i++) {
				
				RE::RaytracingInstance instance;
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


				RWStructuredBuffer vertBuff;
				vertBuff.Init(desc.Triangles.VertexBuffer.StrideInBytes, desc.Triangles.VertexCount, nullptr);
				m_vertexBufferList.push_back(std::move(vertBuff));
			}
			});
	}
	void RaytracingInstance::Build_BLAS()
	{
		auto& rc = g_graphicsEngine->GetRenderContext();
		for (const auto& instance : m_instances)
		{
			D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS inputs = {};
			inputs.DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY;
			inputs.Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_NONE;
			inputs.NumDescs = 1;
			inputs.pGeometryDescs = &instance.geometoryDesc;
			inputs.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL;

			auto d3dDevice = g_graphicsEngine->GetD3DDevice();
			D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO info;
			d3dDevice->GetRaytracingAccelerationStructurePrebuildInfo(&inputs, &info);

			RE::AccelerationStructureBuffers asbuffer;
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
		}
	}
	void RaytracingInstance::Remove()
	{
	}
	void RaytracingInstance::Register()
	{
	}
	void RaytracingInstance::Update(const Matrix& mat, float deltaTime)
	{
	}
	void RaytracingInstance::Update_RaytraceData()
	{
	}
}