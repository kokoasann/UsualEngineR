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
		/// �V�F�[�_�[�e�[�u��
		/// </summary>
		/// <remark>
		/// �V�F�[�_�[�e�[�u���̓��C�̃f�B�X�p�b�`�Ŏg�p�����A�V�F�[�_�[�⃊�\�[�X�̃f�B�X�N���v�^�q�[�v�̃A�h���X�Ȃǂ̃e�[�u���ł��B
		/// </remark>
		class ShaderTable {
		public:
			/// <summary>
			/// �V�F�[�_�[�e�[�u�����������B
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
			ID3D12ResourcePtr m_shaderTable;			//�V�F�[�_�[�e�[�u���B
			uint32_t m_shaderTableEntrySize = 0;
			int m_numRayGenShader = 0;
			int m_numMissShader = 0;
			int m_numHitShader = 0;
		};
	}
}