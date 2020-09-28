#include "PreCompile.h"
#include "Physics/MeshCollider.h"


namespace UER
{
	MeshCollider::MeshCollider()
	{
	}


	MeshCollider::~MeshCollider()
	{

	}

	void MeshCollider::Release()
	{
		auto ptr = m_meshShape.release();
		delete ptr;
		auto ptr1 = m_stridingMeshInterface.release();
		delete ptr1;
		for (auto& p : m_indexBufferArray)
		{
			auto rp = p.release();
			delete rp;
		}
		for (auto& p : m_vertexBufferArray)
		{
			auto rp = p.release();
			delete rp;
		}
		m_indexBufferArray.clear();
		m_vertexBufferArray.clear();
	}

	/*!
	 * @brief	CSkinModelからメッシュコライダーを生成。
	 *@param[in]	model		スキンモデル。
	 */
	void MeshCollider::CreateFromSkinModel(const Model& model, const Matrix* offsetMatrix)
	{
		Matrix mBias = g_matIdentity;
		if (model.GetUpAxis() == enUpAxisZ) {
			//Z-up
			mBias.MakeRotationX(Math::PI * -0.5f);
		}
		//mBias.MakeRotationX(Math::PI * -0.5f);
		if (offsetMatrix != nullptr) {
			mBias = mBias * (*offsetMatrix);
		}
		m_stridingMeshInterface = std::make_unique<btTriangleIndexVertexArray>();

		model.QueryMeshsFromTkm([&](const auto & mesh) {

			//頂点バッファを作成。
			{
				VertexBufferPtr vertexBuffer = std::make_unique<VertexBuffer>();
				Vector3 pos;
				for (const auto& vert: mesh.vertexBuffer) {
					pos = vert.pos;
					//バイアスをかける。
					mBias.Apply(pos);
					vertexBuffer->push_back(pos);
				}
				m_vertexBufferArray.push_back(std::move(vertexBuffer));
			}
			//インデックスバッファを作成。
			{

				//@todo cmoファイルはインデックスバッファのサイズは2byte固定。
				IndexBufferPtr indexBuffer = std::make_unique<IndexBuffer>();


				if (!mesh.indexBuffer32Array.empty())
				{
					for (const auto& indbuff : mesh.indexBuffer32Array)
					{
						for(const auto& ind: indbuff.indices )
							indexBuffer->push_back(ind);
					}
				}
				else
				{
					for (const auto& indbuff : mesh.indexBuffer16Array)
					{
						for (const auto& ind : indbuff.indices)
							indexBuffer->push_back(ind);
					}
				}
				m_indexBufferArray.push_back(std::move(indexBuffer));
			}

			//インデックスメッシュを作成。
			btIndexedMesh indexedMesh;
			IndexBuffer* ib = m_indexBufferArray.back().get();
			VertexBuffer* vb = m_vertexBufferArray.back().get();
			indexedMesh.m_numTriangles = (int)ib->size() / 3;
			indexedMesh.m_triangleIndexBase = (unsigned char*)(&ib->front());
			indexedMesh.m_triangleIndexStride = 12;
			indexedMesh.m_numVertices = (int)vb->size();
			indexedMesh.m_vertexBase = (unsigned char*)(&vb->front());
			indexedMesh.m_vertexStride = sizeof(Vector3);
			m_stridingMeshInterface->addIndexedMesh(indexedMesh);
			}
		);
		m_meshShape = std::make_unique<btBvhTriangleMeshShape>(m_stridingMeshInterface.get(), true);
	}
}