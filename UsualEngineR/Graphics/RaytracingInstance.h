#pragma once


namespace UER
{
	typedef RaytracingEngine RE;
	class RaytracingInstance
	{
	public:
		RaytracingInstance() {}
		virtual ~RaytracingInstance();

		void Release();

		void Build(Model& model);

		void Build_BLAS();

		void Remove();

		void Register();

		void Update(const Matrix& mat,float deltaTime);

		void Update_RaytraceData();
	private:
		int m_id = -1;
		RE::AccelerationStructureBuffers m_bottomLevelAS;	//-B-L-A-S-
		std::vector<RE::RaytracingInstance> m_instances;	//
		std::vector<RWStructuredBuffer> m_vertexBufferList;	//ƒXƒLƒ“—p‚ÌBuffer

		bool m_isUpdate;
		Matrix m_matrix;
	};
}