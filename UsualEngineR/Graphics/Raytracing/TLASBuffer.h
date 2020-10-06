#pragma once
#include "RaytracingInstance.h"


namespace UER
{
	class RenderContext;

	namespace raytracing 
	{
		/// <summary>
		/// TLASBuffer
		/// </summary>
		class TLASBuffer : public IShaderResource
		{
		public:
			/// <summary>
			/// TLASを構築。
			/// </summary>
			/// <param name="rc"></param>
			/// <param name="instances"></param>
			void Init(
				RenderContext& rc,
				const std::vector<InstancePtr>& instances,
				const std::vector< AccelerationStructureBuffers>& bottomLevelASBuffers
			);
			/// <summary>
			/// SRVに登録。
			/// </summary>
			/// <param name="descriptorHandle"></param>
			void RegistShaderResourceView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNo) override;
			/// <summary>
			/// VRAM上の仮想アドレスを取得。
			/// </summary>
			/// <returns></returns>
			D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress()
			{
				return m_topLevelASBuffers.pResult->GetGPUVirtualAddress();
			}
		private:

			AccelerationStructureBuffers m_topLevelASBuffers;
		};
	}//namespace raytracing
}