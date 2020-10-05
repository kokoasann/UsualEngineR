#pragma once
#include "RaytracingInstance.h"

namespace UER
{
	namespace raytracing {
		/// <summary>
		/// BLASBuffer
		/// </summary>
		class BLASBuffer
		{
		public:
			/// <summary>
			/// 初期化。
			/// </summary>
			/// <param name="rc"></param>
			/// <param name="instance"></param>
			void Init(RenderContext& rc, const std::vector<InstancePtr>& instance);
			/// <summary>
			/// BLASBufferのリストを取得。
			/// </summary>
			/// <returns></returns>
			std::vector< AccelerationStructureBuffers>& Get()
			{
				return m_bottomLevelASBuffers;
			}
		private:

			std::vector< AccelerationStructureBuffers> m_bottomLevelASBuffers;	//BLASBuffer
		};
	}//namespace raytracing
}