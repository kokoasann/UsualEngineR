#pragma once
#include "RaytracingWorld.h"
#include "GPUBuffer.h"

namespace UER
{
	namespace raytracing {
		/// <summary>
		/// ���C�g���[�V���O�Ŏg�p����f�B�X�N���v�^�q�[�v
		/// </summary>
		class DescriptorHeaps {
		public:
			/// <summary>
			/// �e�탊�\�[�X���f�B�X�N���v�^�q�[�v�ɓo�^���Ă����B
			/// </summary>
			/// <param name="world"></param>
			/// <param name="outputBuffer"></param>
			/// <param name="rayGeneCB"></param>
			void Init(
				World& world,
				GPUBuffer& outputBuffer,
				ConstantBuffer& rayGeneCB
			);
			const DescriptorHeap& GetSrvUavCbvDescriptorHeap() const
			{
				return m_srvUavCbvHeap;
			}
			const DescriptorHeap& GetSamplerDescriptorHeap() const
			{
				return m_samplerDescriptorHeap;
			}
		private:
			DescriptorHeap m_srvUavCbvHeap;			//SRV_UAV_CBV�p�̃f�B�X�N���v�^�q�[�v�B
			DescriptorHeap m_samplerDescriptorHeap;	//�T���v���p�̃f�B�X�N���v�^�q�[�v�B
		};
	}
}