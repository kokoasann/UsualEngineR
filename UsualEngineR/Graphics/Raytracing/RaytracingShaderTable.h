#pragma once

#include "RaytracingDescriptorHeaps.h"

namespace UER
{
	
	namespace raytracing {
		using ID3D12ResourcePtr = CComPtr<ID3D12Resource>;
		//
		class World;
		class PSO;
		/// <summary>
		/// シェーダーテーブル
		/// </summary>
		/// <remark>
		/// シェーダーテーブルはレイのディスパッチで使用される、シェーダーやリソースのディスクリプタヒープのアドレスなどのテーブルです。
		/// </remark>
		class ShaderTable {
		public:
			/// <summary>
			/// シェーダーテーブルを初期化。
			/// </summary>
			void Init(
				const World& world,
				const PSO& pso,
				const DescriptorHeaps& descriptorHeaps
			);
			ID3D12ResourcePtr Get() const
			{
				return m_shaderTable;
			}
			D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress() const
			{
				return m_shaderTable->GetGPUVirtualAddress();
			}
			uint32_t GetShaderTableEntrySize() const
			{
				return m_shaderTableEntrySize;
			}
			int GetNumRayGenShader() const
			{
				return m_numRayGenShader;
			}
			int GetNumMissShader() const
			{
				return m_numMissShader;
			}
			int GetNumHitShader() const
			{
				return m_numHitShader;
			}
		private:
			ID3D12ResourcePtr m_shaderTable;			//シェーダーテーブル。
			uint32_t m_shaderTableEntrySize = 0;
			int m_numRayGenShader = 0;
			int m_numMissShader = 0;
			int m_numHitShader = 0;
		};
	}
}