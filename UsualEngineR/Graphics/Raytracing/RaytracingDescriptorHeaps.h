#pragma once
#include "RaytracingWorld.h"
#include "GPUBuffer.h"

namespace UER
{
	namespace raytracing {
		/// <summary>
		/// レイトレーシングで使用するディスクリプタヒープ
		/// </summary>
		class DescriptorHeaps {
		public:
			/// <summary>
			/// 各種リソースをディスクリプタヒープに登録していく。
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
			DescriptorHeap m_srvUavCbvHeap;			//SRV_UAV_CBV用のディスクリプタヒープ。
			DescriptorHeap m_samplerDescriptorHeap;	//サンプラ用のディスクリプタヒープ。
		};
	}
}